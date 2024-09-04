import React, { useState } from 'react';
import axios from 'axios';

const Login = ({ onLogin }) => {
    const [formData, setFormData] = useState({
      email: '',
      password: ''
    });
  
    const { email, password } = formData;
  
    const onChange = e => setFormData({ ...formData, [e.target.name]: e.target.value });
  
    const onSubmit = async e => {
      e.preventDefault();
      try {
        const res = await axios.post('/login', formData);
        console.log(res.data);
        localStorage.setItem('token', res.data.token);
        localStorage.setItem('personId', res.data.personId);
        onLogin(); // Call the onLogin function passed as prop
      } catch (err) {
        console.error(err.response?.data);
        alert('Login failed: ' + (err.response?.data?.message || 'Unknown error'));
      }
    };

  return (
    <div>
      <h2>Login</h2>
      <form onSubmit={onSubmit}>
        <input
          type="email"
          placeholder="Email Address"
          name="email"
          value={email}
          onChange={onChange}
          required
        />
        <input
          type="password"
          placeholder="Password"
          name="password"
          value={password}
          onChange={onChange}
          required
        />
        <button type="submit">Login</button>
      </form>
    </div>
  );
};

export default Login;


