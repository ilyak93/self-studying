import React, { useState, useEffect } from 'react';
import axios from 'axios';

const ChatList = ({ onChatSelect, currentUser }) => {
  const [chats, setChats] = useState([]);
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    fetchChats();
  }, []);

  const fetchChats = async () => {
    setIsLoading(true);
    setError(null);
    try {
      const response = await axios.get('/api/chats', {
        headers: { 'Authorization': `Bearer ${localStorage.getItem('token')}` }
      });
      setChats(response.data);
    } catch (error) {
      console.error('Error fetching chats:', error);
      setError('Failed to fetch chats. Please try again later.');
    } finally {
      setIsLoading(false);
    }
  };

  const handleChatSelect = (chat) => {
    const selectedChat = {
      id: chat.id,
      name: chat.type === 'group' ? chat.name : chat.otherPersonName,
      type: chat.type
    };
    onChatSelect(selectedChat);
  };

  if (isLoading) {
    return <div>Loading chats...</div>;
  }

  if (error) {
    return <div>{error}</div>;
  }

  return (
    <div>
      <h2>Your Chats</h2>
      {chats.length === 0 ? (
        <p>No chats available. Start a new chat by searching for a user.</p>
      ) : (
        <ul style={{ listStyleType: 'none', padding: 0 }}>
          {chats.map(chat => (
            <li 
              key={chat.id} 
              onClick={() => handleChatSelect(chat)}
              style={{ cursor: 'pointer', padding: '10px', border: '1px solid #ddd', marginBottom: '5px' }}
            >
              {chat.type === 'group' ? `Group: ${chat.name}` : chat.otherPersonName}
            </li>
          ))}
        </ul>
      )}
    </div>
  );
};

export default ChatList;