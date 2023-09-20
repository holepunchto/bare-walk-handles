#include <assert.h>
#include <bare.h>
#include <js.h>
#include <uv.h>

typedef struct {
  js_env_t *env;
  js_value_t *cb;
} bare_handle_walk_t;

static void
on_bare_walk_handle (uv_handle_t *handle, void *data) {
  int err;

  bare_handle_walk_t *walk = (bare_handle_walk_t *) data;

  js_env_t *env = walk->env;

  js_value_t *result;
  err = js_create_object(env, &result);
  assert(err == 0);

  js_value_t *address;
  err = js_create_bigint_uint64(env, (uintptr_t) handle, &address);
  assert(err == 0);

  err = js_set_named_property(env, result, "address", address);
  assert(err == 0);

#define V(name, value) \
  { \
    js_value_t *val; \
    err = js_create_int32(env, value, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, result, name, val); \
    assert(err == 0); \
  }

  V("type", handle->type)
  V("isActive", uv_is_active(handle))
  V("isClosing", uv_is_closing(handle))
  V("hasRef", uv_has_ref(handle))
#undef V

  js_value_t *global;
  err = js_get_global(env, &global);
  assert(err == 0);

  js_call_function(env, global, walk->cb, 1, &result, NULL);
}

static js_value_t *
bare_walk_handles (js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  uv_loop_t *loop;
  err = js_get_env_loop(env, &loop);
  assert(err == 0);

  bare_handle_walk_t walk = {env, argv[0]};

  uv_walk(loop, on_bare_walk_handle, (void *) &walk);

  return NULL;
}

static js_value_t *
init (js_env_t *env, js_value_t *exports) {
  int err;

#define V(name, fn) \
  { \
    js_value_t *val; \
    err = js_create_function(env, name, -1, fn, NULL, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

  V("walkHandles", bare_walk_handles)
#undef V

  js_value_t *constants;
  err = js_create_object(env, &constants);
  assert(err == 0);

  err = js_set_named_property(env, exports, "constants", constants);
  assert(err == 0);

#define V(name, msg) \
  { \
    js_value_t *val; \
    err = js_create_int32(env, UV_##name, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, constants, #name, val); \
    assert(err == 0); \
  }

  UV_HANDLE_TYPE_MAP(V);
#undef V

  return exports;
}

BARE_MODULE(bare_walk_handles, init)
