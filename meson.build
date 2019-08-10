project('broccolibot_vision', 'cpp', default_options : ['cpp_std=c++11'])

sources = [
    'src/main.cpp',
    'src/broccoli_detector.cpp',
    'src/histogram.cpp',
    'src/pipeline.cpp',
    'src/pipeline_ui.cpp',
]

include_dir = include_directories('inc')

dependencies = [
    dependency('opencv4'),
    #dependency('opencv'),
    dependency('realsense2'),
    #dependency('boost', modules : ['system']),
    #dependency('threads')
]

executable(
    'vision', 
    sources, 
    dependencies : dependencies,
    include_directories : include_dir)
