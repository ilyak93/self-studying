import React, { useState } from 'react';
import axios from 'axios';

const UserSearch = ({ onSelectUser }) => {
  const [searchTerm, setSearchTerm] = useState('');
  const [searchResults, setSearchResults] = useState([]);

  const handleSearch = async () => {
    try {
      const response = await axios.get(`/api/users/search?email=${searchTerm}`, {
        headers: { 'Authorization': `Bearer ${localStorage.getItem('token')}` }
      });
      setSearchResults(response.data);
    } catch (error) {
      console.error('Error searching users:', error);
    }
  };

  return (
    <div>
      <input
        type="text"
        value={searchTerm}
        onChange={(e) => setSearchTerm(e.target.value)}
        placeholder="Search users by email"
      />
      <button onClick={handleSearch}>Search</button>
      <ul>
        {searchResults.map(user => (
          <li key={user.id}>
            {user.name} ({user.email})
            <button onClick={() => onSelectUser(user)}>Chat</button>
          </li>
        ))}
      </ul>
    </div>
  );
};

export default UserSearch;