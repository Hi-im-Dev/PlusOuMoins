# MoreOrLess

A simple server-client more or less game.

## Summary

  - [Getting Started](#getting-started)
  - [Current Features](#current-features)
  - [Missing Features](#missing-features)

## Getting Started

To build the project, first make sure your ENV is correctly setup for a Qt compilation (qmake and make in PATH).

Then run theses commands:
```
$> mkdir build

$> cd build

$> qmake ..

$> make ..
```
And run the associated makefile for each project part (server / client).

## Current Features

Client :
  - Connection to server
  - Profile configuration
  - Animated game
  - Stats of your current game at end

Server :
  - Multiple client instances
  - Multiple options (see --help)
  - JSON storage of game data

## Missing Features

Client :
  - Bot feature
  - Better display for end stats
  - Buttons to navigate independantly on views
  - Replay / Exit button (the close button in top right of window still works, don't worry)

Server :
  - History save (everything is in place, just need implementation)
  - Better error display (custom handler with qInstallMessageHandler)
