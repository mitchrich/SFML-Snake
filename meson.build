project('SFML-Snake', 'cpp', default_options: ['cpp_std=c++23'])

src = [
  'src/main.cpp',
  'src/utility.cpp',
  'src/Button.cpp',
  'src/Player.cpp',
  'src/SnakeGameRenderer.cpp',
  'src/Screen.cpp',
  'src/SnakeModel.cpp',
  'src/SnakeController.cpp'
]

sfml_dep = dependency('sfml', default_options: [
  'audio=false',
  'network=false',
])

incdir = include_directories('include')

executable('snake', src,
  dependencies: [sfml_dep],
  include_directories: [incdir],
)
