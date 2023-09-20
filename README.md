# bare-walk-handles

Walk the event loop handles of the Bare process.

```
npm i bare-walk-handles
```

## Usage

``` js
const walkHandles = require('bare-walk-handles')

for (const handle of walkHandles()) {
  if (handle.type === walkHandles.constants.TIMER) {
    console.log('timer', handle)
  }
}
```

## License

Apache-2.0
