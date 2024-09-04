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
  password: { type: String, required: true }
});

personSchema.set('toJSON', {
  transform: (document, returnedObject) => {
    returnedObject.id = returnedObject.id || returnedObject._id.toString();
    delete returnedObject._id;
    delete returnedObject.__v;
    delete returnedObject.password; // Don't send password to client
  }
});

const groupSchema = new mongoose.Schema({
  id: { type: String, required: true, unique: true },
  name: { type: String, required: true },
  members: [{ type: String, ref: 'Person' }],
});

const messageSchema = new mongoose.Schema({
  id: { type: String, required: true, unique: true },
  content: { type: String, required: true },
  sender: { type: String, ref: 'Person', required: true },
  timestamp: { type: Date, default: Date.now },
  recipientType: { type: String, enum: ['person', 'group'], required: true },
  recipientId: { type: String, required: true }
});

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
    console.error(error);
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
        id: person.id  // Use the string id, not _id
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
  try {
    const personId = req.person.id;
    console.log('Fetching chats for person:', personId);

    const personalChats = await Message.aggregate([
      {
        $match: {
          $or: [{ sender: personId }, { recipientId: personId }]
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
          as: 'personInfo'
        }
      },
      {
        $unwind: '$personInfo'
      },
      {
        $project: {
          _id: 0,
          id: '$_id',
          name: '$personInfo.name',
          type: 'personal',
          lastMessage: 1
        }
      }
    ]);

    console.log('Personal chats found:', personalChats.length);
    res.json(personalChats);
  } catch (error) {
    console.error('Error fetching chats:', error);
    res.status(500).json({ message: 'Error fetching chats', error: error.message });
  }
});

app.get('/api/chats/:chatId/messages', authMiddleware, async (req, res) => {
  try {
    const { chatId } = req.params;
    const personId = req.person.id;

    // For now, just fetch messages without checking chat participation
    const messages = await Message.find({ 
      $or: [
        { sender: personId, recipientId: chatId },
        { sender: chatId, recipientId: personId }
      ]
    })
    .sort({ timestamp: 1 })
    .populate('sender', 'name');

    res.json(messages);
  } catch (error) {
    console.error('Error fetching messages:', error);
    res.status(500).json({ message: 'Error fetching messages' });
  }
});

app.post('/api/chats/:chatId/messages', authMiddleware, async (req, res) => {
  try {
    const { chatId } = req.params;
    const { content } = req.body;
    const personId = req.person.id;

    const newMessage = new Message({
      id: generateUniqueId('M'),
      content,
      sender: personId,
      recipientId: chatId,
      recipientType: 'person', // Assume personal chat for now
      timestamp: new Date()
    });

    await newMessage.save();

    res.status(201).json(newMessage);
  } catch (error) {
    console.error('Error sending message:', error);
    res.status(500).json({ message: 'Error sending message' });
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
  try {
    const { email } = req.query;
    const users = await Person.find({ 
      email: { $regex: email, $options: 'i' },
      id: { $ne: req.person.id } // Exclude the current user
    }).select('id name email');
    res.json(users);
  } catch (error) {
    console.error('Error searching users:', error);
    res.status(500).json({ message: 'Error searching users' });
  }
});

app.post('/api/messages', authMiddleware, async (req, res) => {
  try {
    const { recipientId, content } = req.body;
    const senderId = req.person.id;

    const newMessage = new Message({
      id: generateUniqueId('M'),
      content,
      sender: senderId,
      recipientId,
      recipientType: 'person',
      timestamp: new Date()
    });

    await newMessage.save();

    res.status(201).json(newMessage);
  } catch (error) {
    console.error('Error sending message:', error);
    res.status(500).json({ message: 'Error sending message' });
  }
});

app.get('/api/messages/:recipientId', authMiddleware, async (req, res) => {
  try {
    const { recipientId } = req.params;
    const senderId = req.person.id;

    const messages = await Message.find({
      $or: [
        { sender: senderId, recipientId: recipientId },
        { sender: recipientId, recipientId: senderId }
      ]
    }).sort({ timestamp: 1 });

    res.json(messages);
  } catch (error) {
    console.error('Error fetching messages:', error);
    res.status(500).json({ message: 'Error fetching messages' });
  }
});
