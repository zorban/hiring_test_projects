import { useState } from 'react'
import { useRouter } from 'next/router'
import Login from '../components/Login'

import session from '../session/index'

const users = {
  zbc: {
    id: 1,
    name: 'Zoran Banković',
    password: 'swordpas',
  },
  udo: {
    id: 2,
    name: 'Dr. Udo Brinkmann',
    password: 'udopass',
  },
  pixy: {
    id: 3,
    name: 'Dragan Stojković',
    password: 'pixypass',
  },
  sajo: {
    id: 4,
    name: 'Safet Sušić',
    password: 'sajopass',
  }
}

const login = () => {
  const [message, setMessage] = useState('')
  const router = useRouter()

  const checkUser = (username, password) => {
    setMessage('')
    const user = users[username]
    if (user && user.password === password) {
      delete user.password
      session.start(user);
      router.push('/')
      return true
    }
    setMessage('Invalid login')
    return false
  }

  return (
    <div className='flex-center'>
      <h3>Enter udo/udopass to login :)</h3>
      <Login onLogin={checkUser} />
      {message && <b style={{color: 'red'}}>{message}</b>}
    </div>
  )
}

export default login

export async function getServerSideProps(context) {
  return {
    props: {},
  }
}
