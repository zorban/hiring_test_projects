const USER_KEY = 'user'
const data = {};

const getUser = () => {
  if (!data.user) {
    data.user = JSON.parse(sessionStorage.getItem(USER_KEY))
  }
  return data.user
}

const setUser = (user) => {
  sessionStorage.setItem(USER_KEY, JSON.stringify(user))
}

const active = () => {
  const user = getUser();
  return user && user.name
}

const start = (user) => {
  setUser(user);
}

const end = () => {
  delete data.user
  sessionStorage.clear()
}

export default {
  setUser,
  getUser,
  active,
  start,
  end
}
