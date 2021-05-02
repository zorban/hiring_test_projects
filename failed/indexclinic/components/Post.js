import { useState } from 'react'
import postStyles from '../styles/Post.module.scss'

const Post = ({ post, index, onDelete, onUpdate }) => {
  const [showPopup, setShowPopup] = useState(false)
  const [postString, setPostString] = useState('');
  return (
    <div className={postStyles.card}>
      <h4>{index + 1}. {post.title}</h4>
      <p>{post.body}</p>
      <button className='btn' onClick={() => onDelete(post.id)}>Delete</button>
      <button className='btn' onClick={() => setShowPopup(!showPopup)}>Edit</button>
      { showPopup &&
        <div className={postStyles.popup}>
          <div className={postStyles.editor}>
            <div className={postStyles.close} onClick={() => setShowPopup(!showPopup)}>&#10008;</div>
            <textarea autoFocus cols="50" rows="20" defaultValue={JSON.stringify(post, null, 2)} onChange={e => setPostString(e.target.value)}></textarea>
            <button className='btn' onClick={() => onUpdate(post.id, postString)}>Update</button>
          </div>
        </div>
      }
    </div>
  )
}

export default Post
