
from setuptools import setup
setup(name='iwd',
      version='1.0.0-alpha15',
      packages=['iwd'],
      entry_points={
          'console_scripts': [
              'iwd=iwd.__main__:main'
          ]
      },
      package_data={'': ['schema.json']},
      include_package_data=True,
      install_requires=[
          'jsonschema'
      ]

      )
