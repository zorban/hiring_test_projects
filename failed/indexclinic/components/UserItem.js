import userItemStyles from '../styles/UserItem.module.scss'
import Link from 'next/link'

const UserItem = ({ user }) => {
  return (
    <div className={userItemStyles.card}>
      {/* <pre>
        {JSON.stringify(user, null, 2)}
      </pre> */}
      <h3>{user.name}</h3>
      <p>{user.email}</p>
      <a href={'http://' + user.website}>{user.website}</a>
      <h4>{user.company.name}</h4>
      <p>{user.company.catchPhrase}</p>
      <Link href={'/users/' + user.id}><button className='btn'>View details/Change</button></Link>
    </div>
  )
}

export default UserItem
