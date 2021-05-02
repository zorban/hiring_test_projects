import { useState } from 'react'
import Post from './Post'
import Paginator from './Paginator'
import postStyles from '../styles/Posts.module.scss'

const Posts = ({ posts, onDelete, onUpdate }) => {
  const [startIndex, setStartIndex] = useState(0)
  const [pageLength, setPageLength] = useState(11)

  const renderPage = (startIndex, pageLength) => {
    setStartIndex(startIndex)
    setPageLength(pageLength)
  }
  return (
    <div className={postStyles.container}>
      <Paginator length={posts.length} pageSize={11} onChange={renderPage} />
      {posts.filter((v, i) => i >= startIndex && i < startIndex + pageLength).map((post, ind) => <Post key={post.id} post={post} index={startIndex + ind} onDelete={onDelete} onUpdate={onUpdate} />)}
    </div>
  )
}

export default Posts


