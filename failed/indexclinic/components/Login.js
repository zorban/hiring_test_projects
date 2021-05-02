import { useState } from 'react'
import loginStyles from '../styles/Login.module.scss'

const Login = ({ onLogin }) => {
  const [ username, setUsername ] = useState('')
  const [ password, setPassword ] = useState('')

  const submit = (e) => {
    e.preventDefault();
    onLogin(username, password)
  }

  return (
    <form className={loginStyles.container} onSubmit={submit}>
      <div className={loginStyles.content}>
        <label htmlFor="username">Username</label>
        <input autoFocus type="text" id="username" value={username} onChange={(e) => setUsername(e.target.value)} />
        <label htmlFor="password">Password</label>
        <input type="password" id="password" value={password} onChange={(e) => setPassword(e.target.value)} />
        <input type="submit" value="Login" className='btn'/>
      </div>
    </form>
  )
}

export default Login
