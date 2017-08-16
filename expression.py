#Get sensor data
sensors = {
    "light" : True,
    "rain" : False,
    "temperature" : 22,
    }

print(sensors)

conditions = ""

"""
'print(a+b)'
>>> help(exec)
Help on built-in function exec in module builtins:

exec(source, globals=None, locals=None, /)
    Execute the given source in the context of globals and locals.

    The source may be a string representing one or more Python statements
    or a code object as returned by compile().
    The globals must be a dictionary and locals can be any mapping,
    defaulting to the current globals and locals.
    If only globals is given, locals defaults to it.

>>> d = {"a":1, "b":2}
>>> d
{'a': 1, 'b': 2}
>>> exec(s, locals=d)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: exec() takes no keyword arguments
>>> exec(s, locals=d)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: exec() takes no keyword arguments
>>> exec(s, None, d)
3
>>> d
{'a': 1, 'b': 2}
>>> d
{'a': 1, 'b': 2}
>>> exec(s, None, d)
3
>>>
"""
