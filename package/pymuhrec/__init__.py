import os
import sys

cur_file_dir = os.path.dirname(os.path.realpath(__file__))

sys.path.append(cur_file_dir)
os.chdir(cur_file_dir)

from advfilters import *
from muhrectomo import *
from imgalg import *

