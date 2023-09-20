const binding = require('./binding')

module.exports = exports = function * walkHandles () {
  const handles = []

  binding.walkHandles((handle) => handles.push(new Handle(handle)))

  yield * handles
}

exports.constants = binding.constants

class Handle {
  constructor (opts = {}) {
    this.type = opts.type
    this.isActive = opts.isActive !== 0
    this.isClosing = opts.isClosing !== 0
    this.hasRef = opts.hasRef !== 0
  }
}
