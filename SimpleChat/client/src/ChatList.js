import React, { useState, useEffect } from 'react';
import axios from 'axios';

const ChatList = ({ onChatSelect }) => {
  const [chats, setChats] = useState([]);

  useEffect(() => {
    const fetchChats = async () => {
      try {
        const response = await axios.get('/api/chats', {
          headers: { 'Authorization': `Bearer ${localStorage.getItem('token')}` }
        });
        setChats(response.data);
      } catch (error) {
        console.error('Error fetching chats:', error);
      }
    };

    fetchChats();
  }, []);

  return (
    <div className="chat-list">
      <h2>Your Chats</h2>
      {chats.length === 0 ? (
        <p>No chats to display</p>
      ) : (
        <ul>
          {chats.map(chat => (
            <li key={chat.id} onClick={() => onChatSelect(chat)}>
              <strong>{chat.name}</strong>
              {chat.lastMessage && (
                <p>{chat.lastMessage.content.substring(0, 30)}...</p>
              )}
            </li>
          ))}
        </ul>
      )}
    </div>
  );
};

export default ChatList;