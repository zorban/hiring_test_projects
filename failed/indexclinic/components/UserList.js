import { useState } from 'react'
import UserItem from './UserItem'
import Paginator from './Paginator'
import userListStyles from '../styles/UserList.module.scss'

const UserList = ({ users }) => {
  const [startIndex, setStartIndex] = useState(0)
  const [pageLength, setPageLength] = useState(3)

  const renderList = (startIndex, pageLength) => {
    setStartIndex(startIndex)
    setPageLength(pageLength)
  }

  return (
    <>
      <Paginator length={users.length} pageSize={3} onChange={renderList} />
      <div className={userListStyles.entries}>
        {users.filter((v, i) => i >= startIndex && i < startIndex + pageLength).map(user => (<UserItem key={user.id} user={user} />))}
      </div>
    </>
  )
}

export default UserList
