const test = require('brittle')
const walkHandles = require('.')

test('basic', (t) => {
  const iterator = walkHandles()

  for (const handle of iterator) {
    t.comment(handle)
  }
})
