import layoutStyles from '../styles/Layout.module.scss'
import Nav from './Nav'

const Layout = ({ children }) => {
  return (
    <div className={layoutStyles.container}>
      <Nav />
      <div className={layoutStyles.main}>
        {children}
      </div>
    </div>
  )
}

export default Layout
