from skbuild import setup  # This line replaces 'from setuptools import setup'
setup(
    name="mylib",
    version="0.0.0",
    description="simple getter setter",
    author='Daniele Rapetti',
    license="MIT",
    packages=['mylib'],
    python_requires=">=3.6",
)