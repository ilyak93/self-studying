import React, { useState, useEffect } from 'react';
import axios from 'axios';

const ChatWindow = ({ chat, currentUser }) => {
  const [messages, setMessages] = useState([]);
  const [newMessage, setNewMessage] = useState('');

  useEffect(() => {
    const fetchMessages = async () => {
      try {
        const response = await axios.get(`/api/chats/${chat.id}/messages`, {
          headers: { 'Authorization': `Bearer ${localStorage.getItem('token')}` }
        });
        setMessages(response.data);
      } catch (error) {
        console.error('Error fetching messages:', error);
      }
    };

    if (chat && chat.id) {
      fetchMessages();
    }
  }, [chat]); // Only re-run the effect if `chat` changes

  const sendMessage = async (e) => {
    e.preventDefault();
    if (!newMessage.trim() || !chat || !chat.id) return;

    try {
      const response = await axios.post(`/api/chats/${chat.id}/messages`, {
        content: newMessage,
        recipientType: chat.type
      }, {
        headers: { 'Authorization': `Bearer ${localStorage.getItem('token')}` }
      });
      setMessages([...messages, response.data]);
      setNewMessage('');
    } catch (error) {
      console.error('Error sending message:', error);
    }
  };

  return (
    <div>
      <h2>{chat.type === 'group' ? `Group: ${chat.name}` : `Chat with ${chat.name}`}</h2>
      <div style={{ height: '300px', overflowY: 'scroll', border: '1px solid #ccc' }}>
        {messages.map(message => (
          <div key={message.id}>
            <strong>{message.sender.name}: </strong>
            {message.content}
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