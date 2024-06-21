"""
Configuration for pytest:
  - add cli option pathToApplication to add path of netcatcpp or pipecatcpp
"""


def pytest_addoption(parser):
    parser.addoption("--publisher", type=str, help="Path to publisher executable")
