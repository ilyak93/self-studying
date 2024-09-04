import React, { useState, useEffect } from 'react';
import axios from 'axios';

const ChatWindow = ({ recipient }) => {
  const [messages, setMessages] = useState([]);
  const [newMessage, setNewMessage] = useState('');

  useEffect(() => {
    fetchMessages();
  }, [recipient]);

  const fetchMessages = async () => {
    try {
      const response = await axios.get(`/api/messages/${recipient.id}`, {
        headers: { 'Authorization': `Bearer ${localStorage.getItem('token')}` }
      });
      setMessages(response.data);
    } catch (error) {
      console.error('Error fetching messages:', error);
    }
  };

  const sendMessage = async (e) => {
    e.preventDefault();
    try {
      await axios.post('/api/messages', 
        { recipientId: recipient.id, content: newMessage },
        { headers: { 'Authorization': `Bearer ${localStorage.getItem('token')}` } }
      );
      setNewMessage('');
      fetchMessages();
    } catch (error) {
      console.error('Error sending message:', error);
    }
  };

  const formatDate = (date) => {
    return new Date(date).toLocaleString();
  };

  return (
    <div>
      <h2>Chat with {recipient.name}</h2>
      <div style={{ height: '300px', overflowY: 'scroll' }}>
        {messages.map(message => (
          <div key={message.id}>
            <strong>{formatDate(message.timestamp)} {message.sender.name}:</strong> {message.content}
          </div>
        ))}
      </div>
      <form onSubmit={sendMessage}>
        <input
          type="text"
          value={newMessage}
          onChange={(e) => setNewMessage(e.target.value)}
          placeholder="Type a message..."
        />
        <button type="submit">Send</button>
      </form>
    </div>
  );
};

export default ChatWindow;