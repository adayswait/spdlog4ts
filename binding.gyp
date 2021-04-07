{
  "targets": [
    {
      "target_name": "spdlog4ts",
      'sources': [ 'src/main.cc','src/logger.cc' ],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")", "deps"],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
    }
  ]
}
