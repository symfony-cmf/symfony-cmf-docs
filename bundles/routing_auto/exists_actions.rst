.. index::
    single: Exists Actions; RoutingAutoBundle

Path Exists Actions
-------------------

These are the default actions available to take if the path provided by a
``path_provider`` already exists and so creating a new path would create a
conflict.

auto_increment
~~~~~~~~~~~~~~

The ``auto_increment`` action will add a numerical suffix to the path, for
example ``my/path`` would first become ``my/path-1`` and if that path *also*
exists it will try ``my/path-2``, ``my/path-3`` and so on into infinity until
it finds a path which *doesn't* exist.

This action should typically be used in the ``content_name`` builder unit to
resolve conflicts. Using it in the ``content_path`` builder chain would not
make much sense.

.. configuration-block::

    .. code-block:: yaml

        exists_action: auto_increment

    .. code-block:: xml

        <exists-action name="auto_increment" />

    .. code-block:: php

        array(
            // ...
            'exists_action' => 'auto_increment',
        );

use
~~~

The ``use`` action will simply take the existing path and use it. For example,
in a forum the builder unit must first determine the category path, ``/my-category``,
if this path exists (and it should) then it will be *used* in the stack.

This action should typically be used in one of the content path builder units
to specify that it should use the existing route. On the other hand, using
this as the content name builder action should cause the old route to be
overwritten.

.. configuration-block::

    .. code-block:: yaml

        exists_action: use

    .. code-block:: xml

        <exists-action name="use" />

    .. code-block:: php

        array(
            // ...
            'exists_action' => 'use',
        );
