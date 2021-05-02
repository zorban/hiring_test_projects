import { useRouter } from 'next/router'
import Layout from '../../components/Layout'
import Posts from '../../components/Posts'
import config from '../../config/index'

const posts = ({ posts }) => {
  const router = useRouter()
  const deletePost = async postId => {
    const res = await fetch(`${config.backendBase}/posts/${postId}`, {
      method: 'DELETE'
    })
    const out = await res.json()
    router.reload()
    return out;
  }
  
  const updatePost = async (postId, updatedPostJsonString) => {
    const res = await fetch(`${config.backendBase}/posts/${postId}`, {
      method: 'PUT',
      headers: {
        'Content-type': 'application/json'
      },
      body: updatedPostJsonString
    })
    const out = await res.json()
    router.reload()
    return out;
  }

  return (
    <Layout>
      <Posts posts={posts} onDelete={deletePost} onUpdate={updatePost}/>
    </Layout>
  )
}

export default posts

export const getStaticProps = async (context) => {
  const res = await fetch(`${config.backendBase}/posts`)
  const posts = await res.json()

  return {
    props: {
      posts: posts
    }
  }
}
