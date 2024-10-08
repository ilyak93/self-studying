import React, { useState, useEffect } from 'react';
import axios from 'axios';
import Login from './Login';
import Register from './Register';
import ChatList from './ChatList';
import ChatWindow from './ChatWindow';
import CreateGroup from './CreateGroup';
import UserSearch from './UserSearch';

function App() {
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [showRegister, setShowRegister] = useState(false);
  const [selectedChat, setSelectedChat] = useState(null);
  const [showCreateGroup, setShowCreateGroup] = useState(false);
  const [currentUser, setCurrentUser] = useState(null);

  useEffect(() => {
    const token = localStorage.getItem('token');
    if (token) {
      setIsLoggedIn(true);
      fetchCurrentUser();
    }
  }, []);

  const fetchCurrentUser = async () => {
    try {
      const response = await axios.get('/api/person', {
        headers: { 'Authorization': `Bearer ${localStorage.getItem('token')}` }
      });
      setCurrentUser(response.data);
    } catch (error) {
      console.error('Error fetching current user:', error);
    }
  };

  const handleLogin = () => {
    setIsLoggedIn(true);
    fetchCurrentUser();
  };

  const handleLogout = () => {
    localStorage.removeItem('token');
    setIsLoggedIn(false);
    setCurrentUser(null);
    setSelectedChat(null);
  };

  const handleSelectUser = (user) => {
    setSelectedChat({
      id: user.id,
      name: user.name,
      type: 'personal'
    });
  };

  const handleChatSelect = (chat) => {
    setSelectedChat(chat);
    setShowCreateGroup(false);
  };

  const handleGroupCreated = (newGroup) => {
    setShowCreateGroup(false);
    setSelectedChat({
      id: newGroup.id,
      name: newGroup.name,
      type: 'group'
    });
  };

  const handleCloseCreateGroup = () => {
    setShowCreateGroup(false);
  };

  if (!isLoggedIn) {
    return (
      <div>
        {showRegister ? (
          <Register onRegister={() => setShowRegister(false)} />
        ) : (
          <Login onLogin={handleLogin} />
        )}
        <button onClick={() => setShowRegister(!showRegister)}>
          {showRegister ? 'Switch to Login' : 'Switch to Register'}
        </button>
      </div>
    );
  }

  return (
    <div className="app">
      <header>
        <h1>Messenger App</h1>
        {currentUser && <p>Welcome, {currentUser.name}</p>}
        <button onClick={handleLogout}>Logout</button>
        <button onClick={() => setShowCreateGroup(true)}>Create Group Chat</button>
      </header>
      <div style={{ display: 'flex' }}>
        <div style={{ width: '30%', padding: '20px' }}>
          <UserSearch onSelectUser={handleSelectUser} />
          <ChatList onChatSelect={handleChatSelect} currentUser={currentUser} />
        </div>
        <div style={{ width: '70%', padding: '20px' }}>
          {showCreateGroup ? (
            <CreateGroup 
              onGroupCreated={handleGroupCreated} 
              onClose={handleCloseCreateGroup}
            />
          ) : selectedChat ? (
            <ChatWindow chat={selectedChat} currentUser={currentUser} />
          ) : (
            <p>Select a chat or search for a user to start messaging</p>
          )}
        </div>
      </div>
    </div>
  );
}

export default App;