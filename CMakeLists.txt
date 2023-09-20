cmake_minimum_required(VERSION 3.25)

project(bare_walk_handles C)

include(bare)

add_bare_module(bare_walk_handles)

target_sources(
  bare_walk_handles
  PRIVATE
    binding.c
)
