# Architecture
## Intro

This document explains the **whats** and **whys** of the architecture decisions for **MochiEngine**.

First of all, note that this is a **portfolio** project. This project is meant to show my qualities as an engineer, and I didn't focus (or I actually neglected) several
features or processes that didn't help that purpose.

The most important thing to note is that this is not a general purpose engine. **MochiEngine** is meant to be a 2D Pixel Art engine. Most of the non mandatory features it has are there because one or more of the created samples required them. Others are there because I feel they improve the creation and iteration pipelines.

What I actually wanted **MochiEngine** to do:

- Be simple.
- Be fast.
- Be testable.
- Be extendible.
- Allow to be used in several ways, with the same results.
- Be cross platform.

At the time by the first sample (Space Shooter) was finished, I think all of those points are already true. The next samples are just proving that the engine can be used for a variety of games.

## Interface or not to interface

One of the things I considered when creating this engine was what to make an interface and what not. Which modules could be interchanged? What made sense to switch on runtime?

The engine started with few interfaces: just have the job done as fast as i could. After I had a minimal renderer, input, files and audio systems working on a well manageable codebase, I started focused on testing those modules.

As some modules used submodules or other type of classes (like how the textures are created with a texture factory, or how the audio system read files from the filesystem) I started to notice patterns and making an interface only for what i needed: if I wanted to test how a module reacted to an action, I needed a dummy action manager that did what the test needed. Or later, when I wanted to test full loops on the engine, i didn't need the renderer to show anything, i just needed to check the game state.

That way, I made interfaces of what needed to be *faked*, and left other systems as simple as i could.

## The renderer

## Action system

## Scripting