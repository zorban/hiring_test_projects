import Link from 'next/link'
import { useState } from 'react'
import config from '../../../config/index'
import Layout from '../../../components/Layout'
import User from '../../../components/User'

const user = ({ user }) => {
  let updUser = Object.assign({}, user)
  delete updUser.id
  const [noIdUser, setNoIdUser] = useState(updUser)

  return (
    <Layout>
      <div style={{flexGrow: 1}}>
        <User user={noIdUser} onChange={updatedUser => updateUser(user.id, updatedUser)} onDelete={deleteUser}/>
        <hr/>
        <Link href='/users/'>Go Back</Link>
      </div>
    </Layout>
  )
}

export default user

const deleteUser = async userId => {
  const res = await fetch(`${config.backendBase}/users/${userId}`, {
    method: 'DELETE'
  })
  return await res.json()
}

const updateUser = async (id, updatedUser) => {
  const res = await fetch(`${config.backendBase}/users/${id}`, {
    method: 'PUT',
    headers: {
      'Content-type': 'application/json'
    },
    body: JSON.stringify(updatedUser)
  })
  return await res.json()
}

export const getStaticProps = async (context) => {
  const res = await fetch(`${config.backendBase}/users/${context.params.id}`)
  const user = await res.json()

  return {
    props: {
      user
    }
  }
}

export const getStaticPaths = async () => {
  const res = await fetch(`${config.backendBase}/users`)
  const users = await res.json();
  const paths = users.map(user => ({params: {id: user.id.toString()}}))

  return {
    paths,
    fallback: false
  }
}