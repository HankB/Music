# Music

## Purpose

Exercise the EventFramework library and produce music on an Arduino.
I have used this as a platform to develop more music for another project.

## Requirements

Install the Event Library (<https://github.com/HankB/EventFramework>)

## Variables and macros of interest

`NO_PPROGMEM` controls some code that puts the music in ROM. Otherwise it
goes into RAM which is much more limited.
`speaker1` determines the pin used to output the tone.
`measure` determines the length of a measure of music. Larger values result
in slower play which is useful for debugging a melody.

## Errata

You may not want to leave this on your Arduino if you keep the speaker
connected (and if you leave the speaker connected) as it will then play
every time you apply power to the Arduino.

