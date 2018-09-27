#!/usr/bin/env python3

"""Instant GLSL.

Usage:
  shaderwatch.py watch <file>

Options:

"""
from docopt import docopt
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
import time
import subprocess

import os

file_path = ""
file_dir = ""
push_dir = "/sdcard"


def sh(command):
    p = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    print(p.stdout.read())


def push_shader():
    command = "adb push " + file_path + " " + push_dir
    sh(command)


def send_broadcast():
    command = "adb shell am broadcast " \
              + "-a com.glumes.instant.image.shader " \
              + "--es shaderPath " \
              + os.path.basename(file_path)

    print(command)
    sh(command)


class ShaderHandler(FileSystemEventHandler):
    create_path = ""
    modified_path = ""

    def on_modified(self, event):
        self.modified_path = event.src_path
        if self.modified_path == file_path or self.create_path == file_path:
            push_shader()
            send_broadcast()

    def on_created(self, event):
        self.create_path = event.src_path


if __name__ == '__main__':

    arguments = docopt(__doc__, version='Instant GLSL 2.0')

    file_path = os.path.abspath(arguments['<file>'])

    file_dir = os.path.dirname(file_path) + "/"

    print(file_path)
    print(file_dir)

    event_handler = ShaderHandler()

    observer = Observer()

    observer.schedule(event_handler, file_dir, recursive=True)

    observer.start()
    print("start watch!!!")
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()

    observer.join()

    print("\nstop watch~~~")
