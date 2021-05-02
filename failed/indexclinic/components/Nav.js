import { useEffect } from 'react'
import Link from 'next/link'
import { useRouter } from 'next/router'
import navStyles from '../styles/Nav.module.scss'

import session from '../session/index'

const Nav = () => {
  const router = useRouter()
  useEffect(() => {
    if (!session.active()) {
      router.push('/login')
    }
  }, [])

  const logout = () => {
    session.end()
    router.push('/login')
    // router.reload()
  }

  return (
    <div className={navStyles.nav}>
      <nav>
        <ul>
          <li>
            <Link href='/'>Home</Link>
          </li>
          <li>
            <Link href='/users'>Users</Link>
          </li>
          <li>
            <Link href='/posts'>Posts</Link>
          </li>
        </ul>
      </nav>
      <div>
        <button onClick={logout} className='btn'>&#10149;</button>
      </div>
    </div>
  )
}

export default Nav
