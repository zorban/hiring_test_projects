import { useState } from 'react'
import users from '../pages/users'

import userStyles from '../styles/User.module.scss'

const User = ({ user, onChange, onDelete }) => {
  const [ updatedUser, setUpdatedUser ] = useState(user)
  const [ parseError, setParseError ] = useState('')
  const [ showEditor, setShowEditor ] = useState(false)
  const checkJson = (value) => {
    setParseError('')
    try {
      setUpdatedUser(JSON.parse(value))
    } catch (err) {
      setParseError(err.message)
    }
  }

  return (
    <div className={userStyles.container}> 
      { showEditor ?
        <div className={userStyles.editor}>
          <textarea autoFocus defaultValue={JSON.stringify(updatedUser, null, 2)} onChange={e => checkJson(e.target.value)} />
          {parseError && <p>{parseError}</p>}
        </div>
        :
        <pre className={userStyles.viewer}>
          {JSON.stringify(user, null, 2)}
        </pre>
      }
      <div className={userStyles.buttons}>
        {showEditor ? 
          <button onClick={() => onChange(updatedUser)} className='btn'>Update</button>
        :
          <button onClick={() => onDelete(user.id)} className='btn'>Delete</button>
        }
        <button onClick={() => setShowEditor(!showEditor)} className='btn'>{showEditor ? 'Cancel editing' : 'Edit'}</button>
      </div>
    </div>
  )
}

export default User
