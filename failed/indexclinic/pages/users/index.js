import config from '../../config/index'
import Layout from '../../components/Layout'
import UserList from '../../components/UserList'

const users = ({ users }) => {
  return (
    <Layout>
      <UserList users={users} />
    </Layout>
  )
}

export default users

export const getStaticProps = async (context) => {
  const res = await fetch(`${config.backendBase}/users`)
  const users = await res.json()

  return {
    props: {
      users
    }
  }
}
