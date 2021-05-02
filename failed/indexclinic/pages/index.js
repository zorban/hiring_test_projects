import { useEffect } from 'react'
import { useState } from 'react'

import session from '../session/index'
import Layout from '../components/Layout'

const Page = () => {
  const [user, setUser] = useState(null)

  useEffect(() => {
    setUser(session.getUser())
  }, [])

  return (
    <Layout>
      <div className='flex-center'>
        <h1>Welcome,</h1>
        <h1>{user && user.name}</h1>
        <img src='https://picsum.photos/450/300'></img>
      </div>
    </Layout>
  )
}

export default Page
