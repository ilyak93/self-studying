import React, { useState } from 'react';
import axios from 'axios';

const CreateGroup = ({ onGroupCreated, onClose }) => {
  const [groupName, setGroupName] = useState('');
  const [searchTerm, setSearchTerm] = useState('');
  const [searchResults, setSearchResults] = useState([]);
  const [selectedMembers, setSelectedMembers] = useState([]);

  const searchUsers = async () => {
    if (!searchTerm.trim()) {
      console.log('Search term is empty');
      return;
    }
    
    console.log('Searching for:', searchTerm);
    try {
      const token = localStorage.getItem('token');
      const response = await axios.get('/api/users/search', {
        params: { email: searchTerm },
        headers: { 'Authorization': `Bearer ${token}` }
      });
      console.log('Search response:', response.data);
      setSearchResults(response.data);
    } catch (error) {
      console.error('Error searching users:', error);
      if (error.response) {
        console.error('Error response:', error.response.data);
        console.error('Error status:', error.response.status);
        console.error('Error headers:', error.response.headers);
      } else if (error.request) {
        console.error('Error request:', error.request);
      } else {
        console.error('Error message:', error.message);
      }
    }
  };

  const addMember = (user) => {
    setSelectedMembers([...selectedMembers, user]);
    setSearchResults(searchResults.filter(u => u.id !== user.id));
  };

  const removeMember = (user) => {
    setSelectedMembers(selectedMembers.filter(u => u.id !== user.id));
    setSearchResults([...searchResults, user]);
  };

  const createGroup = async () => {
    if (!groupName.trim()) {
      console.log('Group name is empty');
      return;
    }
    if (selectedMembers.length === 0) {
      console.log('No members selected');
      return;
    }
    try {
      const token = localStorage.getItem('token');
      const response = await axios.post('/api/groups', {
        name: groupName,
        members: selectedMembers.map(m => m.id)
      }, {
        headers: { 'Authorization': `Bearer ${token}` }
      });
      console.log('Group created:', response.data);
      onGroupCreated(response.data);
      onClose();
    } catch (error) {
      console.error('Error creating group:', error);
      if (error.response) {
        console.error('Error response:', error.response.data);
        console.error('Error status:', error.response.status);
        console.error('Error headers:', error.response.headers);
      } else if (error.request) {
        console.error('Error request:', error.request);
      } else {
        console.error('Error message:', error.message);
      }
    }
  };

  return (
    <div>
      <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
        <h2>Create Group Chat</h2>
        <button onClick={onClose}>Close</button>
      </div>
      <input
        type="text"
        value={groupName}
        onChange={(e) => setGroupName(e.target.value)}
        placeholder="Enter group name"
      />
      <div>
        <input
          type="text"
          value={searchTerm}
          onChange={(e) => setSearchTerm(e.target.value)}
          placeholder="Search users by name or email"
        />
        <button onClick={searchUsers}>Search</button>
      </div>
      <div>
        <h3>Search Results:</h3>
        {searchResults.map(user => (
          <div key={user.id}>
            {user.name} ({user.email}) <button onClick={() => addMember(user)}>Add</button>
          </div>
        ))}
      </div>
      <div>
        <h3>Selected Members:</h3>
        {selectedMembers.map(user => (
          <div key={user.id}>
            {user.name} <button onClick={() => removeMember(user)}>Remove</button>
          </div>
        ))}
      </div>
      <button onClick={createGroup}>Create Group</button>
    </div>
  );
};

export default CreateGroup;