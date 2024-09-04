const express = require('express');
const mongoose = require('mongoose');
const bcrypt = require('bcryptjs');
const jwt = require('jsonwebtoken');
const http = require('http');
const socketIo = require('socket.io');
const authMiddleware = require('./middleware/auth');

const app = express();
const server = http.createServer(app);
const io = socketIo(server);

mongoose.connect('mongodb://localhost/messenger', { useNewUrlParser: true, useUnifiedTopology: true });

// Models
const personSchema = new mongoose.Schema({
  id: { type: String, required: true, unique: true },
  name: { type: String, required: true },
  email: { type: String, required: true, unique: true },
  password: { type: String, required: true },
  groups: [{ type: String, ref: 'Group' }]
});

personSchema.set('toJSON', {
  transform: (document, returnedObject) => {
    delete returnedObject.__v;
    delete returnedObject.password;
  }
});

const groupSchema = new mongoose.Schema({
  id: { type: String, required: true, unique: true },
  name: { type: String, required: true },
  members: [{ type: String, ref: 'Person' }]
});

groupSchema.set('toJSON', {
  transform: (document, returnedObject) => {
    returnedObject.id = returnedObject.id || returnedObject._id.toString();
    delete returnedObject._id;
    delete returnedObject.__v;
  }
});

const messageSchema = new mongoose.Schema({
  id: { type: String, required: true, unique: true },
  content: { type: String, required: true },
  sender: { type: String, required: true },
  timestamp: { type: Date, default: Date.now },
  recipientType: { type: String, enum: ['person', 'group'], required: true },
  recipientId: { type: String, required: true }
});

messageSchema.set('toJSON', {
  transform: (document, returnedObject) => {
    returnedObject.id = returnedObject.id || returnedObject._id.toString();
    delete returnedObject._id;
    delete returnedObject.__v;
  }
});

personSchema.index({ name: 'text', email: 'text' });

const Person = mongoose.model('Person', personSchema);
const Group = mongoose.model('Group', groupSchema);
const Message = mongoose.model('Message', messageSchema);

// Middleware
app.use(express.json());

// Routes
app.post('/register', async (req, res) => {
  try {
    const { name, email, password } = req.body;
    
    // Check if user already exists
    const existingUser = await Person.findOne({ email });
    if (existingUser) {
      return res.status(400).json({ message: 'User already exists' });
    }

    // Hash password
    const salt = await bcrypt.genSalt(10);
    const hashedPassword = await bcrypt.hash(password, salt);

    // Create new user
    const newPerson = new Person({
      id: generateUniqueId('P'),
      name,
      email,
      password: hashedPassword
    });

    await newPerson.save();

    res.status(201).json({ message: 'User registered successfully', userId: newPerson.id });
  } catch (error) {
    console.error('Registration error:', error);
    res.status(500).json({ message: 'Server error' });
  }
});

app.post('/login', async (req, res) => {
  try {
    const { email, password } = req.body;
    
    const person = await Person.findOne({ email });
    if (!person) {
      return res.status(400).json({ message: 'Invalid credentials' });
    }

    const isMatch = await bcrypt.compare(password, person.password);
    if (!isMatch) {
      return res.status(400).json({ message: 'Invalid credentials' });
    }

    const payload = {
      person: {
        id: person.id
      }
    };

    jwt.sign(
      payload,
      'your_jwt_secret',
      { expiresIn: '1h' },
      (err, token) => {
        if (err) throw err;
        res.json({ token, personId: person.id });
      }
    );
  } catch (error) {
    console.error('Login error:', error);
    res.status(500).json({ message: 'Server error' });
  }
});



// Socket.io
io.on('connection', (socket) => {
  console.log('New client connected');
  
  socket.on('join room', (room) => {
    socket.join(room);
  });

  socket.on('leave room', (room) => {
    socket.leave(room);
  });

  socket.on('new message', (message) => {
    io.to(message.recipientId).emit('new message', message);
  });

  socket.on('disconnect', () => {
    console.log('Client disconnected');
  });
});

const PORT = process.env.PORT || 3000;
server.listen(PORT, () => console.log(`Server running on port ${PORT}`));

// Helper function to generate unique IDs
function generateUniqueId(prefix) {
  return `${prefix}_${new mongoose.Types.ObjectId().toString()}`;
}

app.get('/api/chats', authMiddleware, async (req, res) => {
  console.log('Received request for /api/chats');
  console.log('req.person:', req.person);
  
  try {
    if (!req.person || !req.person.id) {
      console.log('Person not authenticated properly');
      return res.status(401).json({ message: 'Person not authenticated properly' });
    }

    const personId = req.person.id;
    console.log('Fetching chats for person:', personId);

    // Fetch the person to get their group list
    const currentPerson = await Person.findOne({ id: personId });
    if (!currentPerson) {
      return res.status(404).json({ message: 'Person not found' });
    }

    // Fetch group chats
    const groupChats = await Group.find({ id: { $in: currentPerson.groups } })
      .select('id name')
      .lean();

    const formattedGroupChats = groupChats.map(group => ({
      id: group.id,
      name: group.name,
      type: 'group'
    }));

    console.log('Group chats found:', formattedGroupChats.length);

    // Fetch personal chats
    const personalChats = await Message.aggregate([
      {
        $match: {
          $or: [{ sender: personId }, { recipientId: personId }],
          recipientType: 'person'
        }
      },
      {
        $sort: { timestamp: -1 }
      },
      {
        $group: {
          _id: {
            $cond: [
              { $eq: ['$sender', personId] },
              '$recipientId',
              '$sender'
            ]
          },
          lastMessage: { $first: '$$ROOT' }
        }
      },
      {
        $lookup: {
          from: 'persons',
          localField: '_id',
          foreignField: 'id',
          as: 'otherPerson'
        }
      },
      {
        $unwind: '$otherPerson'
      },
      {
        $project: {
          id: '$_id',
          name: '$otherPerson.name',
          type: 'personal',
          lastMessage: '$lastMessage.content'
        }
      }
    ]);

    console.log('Personal chats found:', personalChats.length);

    // Combine and send all chats
    const allChats = [...formattedGroupChats, ...personalChats];
    console.log('Total chats found:', allChats.length);
    res.json(allChats);
  } catch (error) {
    console.error('Error fetching chats:', error);
    res.status(500).json({ message: 'Error fetching chats' });
  }
});


app.get('/api/chats/:chatId/messages', authMiddleware, async (req, res) => {
  try {
    const { chatId } = req.params;
    const personId = req.person.id;

    // Check if it's a group chat
    const group = await Group.findOne({ id: chatId });
    
    let messages;
    if (group) {
      messages = await Message.find({ 
        recipientId: chatId, 
        recipientType: 'group' 
      }).sort({ timestamp: 1 });
    } else {
      messages = await Message.find({
        $or: [
          { sender: personId, recipientId: chatId, recipientType: 'person' },
          { sender: chatId, recipientId: personId, recipientType: 'person' }
        ]
      }).sort({ timestamp: 1 });
    }

    // Fetch sender names
    const senderIds = [...new Set(messages.map(m => m.sender))];
    const senders = await Person.find({ id: { $in: senderIds } }).select('id name');
    const senderMap = senders.reduce((map, sender) => {
      map[sender.id] = sender.name;
      return map;
    }, {});

    const messagesWithDetails = messages.map(message => ({
      id: message.id,
      content: message.content,
      sender: message.sender,
      senderName: senderMap[message.sender],
      timestamp: message.timestamp,
      recipientType: message.recipientType,
      recipientId: message.recipientId
    }));

    res.json(messagesWithDetails);
  } catch (error) {
    console.error('Error fetching messages:', error);
    res.status(500).json({ message: 'Error fetching messages' });
  }
});



app.get('/api/person', authMiddleware, async (req, res) => {
  try {
    const person = await Person.findOne({ id: req.person.id }).select('-password');
    if (!person) {
      return res.status(404).json({ message: 'Person not found' });
    }
    res.json(person);
  } catch (error) {
    console.error('Error fetching person info:', error);
    res.status(500).json({ message: 'Server error' });
  }
});


app.get('/api/users/search', authMiddleware, async (req, res) => {
  console.log('Search query received:', req.query);
  try {
    const { email } = req.query;

    console.log('Search query received:', email);

    const users = await Person.find({ 
      $or: [
        { email: { $regex: email, $options: 'i' } }
      ],
      id: { $ne: req.person.id } // Exclude the current user
    }).select('id name email');

    console.log('Users found:', users.length);
    res.json(users);
  } catch (error) {
    console.error('Error searching users:', error);
    res.status(500).json({ message: 'Error searching users', error: error.message });
  }
});

app.post('/api/chats/:chatId/messages', authMiddleware, async (req, res) => {
  try {
    const { chatId } = req.params;
    const { content } = req.body;
    const personId = req.person.id;

    // Check if it's a group chat
    const group = await Group.findOne({ id: chatId });
    const recipientType = group ? 'group' : 'person';

    const newMessage = new Message({
      id: generateUniqueId('M'),
      content,
      sender: personId,
      recipientId: chatId,
      recipientType,
      timestamp: new Date()
    });

    await newMessage.save();

    // Fetch sender's name
    const sender = await Person.findOne({ id: personId }).select('name');

    const messageWithSenderName = {
      ...newMessage.toJSON(),
      senderName: sender.name
    };

    res.status(201).json(messageWithSenderName);
  } catch (error) {
    console.error('Error sending message:', error);
    res.status(500).json({ message: 'Error sending message' });
  }
});


app.get('/api/chats/:chatId/messages', authMiddleware, async (req, res) => {
  try {
    const { chatId } = req.params;
    const personId = req.person.id;

    // Check if it's a group chat
    const group = await Group.findOne({ id: chatId });
    
    let messages;
    if (group) {
      messages = await Message.find({ 
        recipientId: chatId, 
        recipientType: 'group' 
      }).sort({ timestamp: 1 });
    } else {
      messages = await Message.find({
        $or: [
          { sender: personId, recipientId: chatId, recipientType: 'person' },
          { sender: chatId, recipientId: personId, recipientType: 'person' }
        ]
      }).sort({ timestamp: 1 });
    }

    // Fetch sender names
    const senderIds = [...new Set(messages.map(m => m.sender))];
    const senders = await Person.find({ id: { $in: senderIds } }).select('id name');
    const senderMap = senders.reduce((map, sender) => {
      map[sender.id] = sender.name;
      return map;
    }, {});

    const messagesWithDetails = messages.map(message => ({
      id: message.id,
      content: message.content,
      sender: message.sender,
      senderName: senderMap[message.sender],
      timestamp: message.timestamp,
      recipientType: message.recipientType,
      recipientId: message.recipientId
    }));

    res.json(messagesWithDetails);
  } catch (error) {
    console.error('Error fetching messages:', error);
    res.status(500).json({ message: 'Error fetching messages' });
  }
});


app.post('/api/groups', authMiddleware, async (req, res) => {
  try {
    const { name, members } = req.body;
    const creatorId = req.person.id;

    const newGroup = new Group({
      id: generateUniqueId('G'),
      name,
      members: [creatorId, ...members]
    });

    await newGroup.save();

    // Update the groups field for all members
    await Person.updateMany(
      { id: { $in: newGroup.members } },
      { $addToSet: { groups: newGroup.id } }
    );

    res.status(201).json(newGroup);
  } catch (error) {
    console.error('Error creating group:', error);
    res.status(500).json({ message: 'Error creating group' });
  }
});
