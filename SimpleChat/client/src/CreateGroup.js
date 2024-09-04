import React, { useState } from 'react';
import axios from 'axios';

const CreateGroup = ({ onGroupCreated }) => {
  const [groupName, setGroupName] = useState('');
  const [searchTerm, setSearchTerm] = useState('');
  const [searchResults, setSearchResults] = useState([]);
  const [selectedMembers, setSelectedMembers] = useState([]);

  const searchUsers = async () => {
    try {
      const response = await axios.get(`/api/users/search?email=${searchTerm}`, {
        headers: { 'Authorization': `Bearer ${localStorage.getItem('token')}` }
      });
      setSearchResults(response.data);
    } catch (error) {
      console.error('Error searching users:', error);
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

  const handleCreateGroup = async (e) => {
    e.preventDefault();
    try {
      const response = await axios.post('/api/groups', {
        name: groupName,
        members: selectedMembers.map(m => m.id)
      }, {
        headers: { 'Authorization': `Bearer ${localStorage.getItem('token')}` }
      });
      onGroupCreated(response.data);
      // Reset form
      setGroupName('');
      setSelectedMembers([]);
      setSearchResults([]);
      setSearchTerm('');
    } catch (error) {
      console.error('Error creating group:', error);
    }
  };

  return (
    <div>
      <h2>Create Group Chat</h2>
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
          placeholder="Search users by email"
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
      <button onClick={handleCreateGroup}>Create Group</button>
    </div>
  );
};

export default CreateGroup;