import json

from .configuration import Configuration
from .requirement import Requirement


class JsonEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, Configuration):
            return obj.data
        if isinstance(obj, Requirement):
            return obj.__dict__
        if callable(getattr(obj, 'to_dict', None)):
            return obj.to_dict()
        return json.JSONEncoder.default(self, obj)
