import json

from .configuration import Configuration
from .requirement import Requirement


class JsonEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, Configuration):
            return obj.data
        if isinstance(obj, Requirement):
            return obj.__dict__
        return json.JSONEncoder.default(self, obj)
