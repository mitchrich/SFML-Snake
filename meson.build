project('SFML-Snake', 'cpp')

src = [
  'src/main.cpp',
  'src/utility.cpp',
  'src/Button.cpp',
  'src/AssetHandler.cpp',
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
executable('snake', src, include_directories: incdir, dependencies: sfml_dep)
