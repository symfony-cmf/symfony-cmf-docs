.. index::
    single: Not Exists Actions; RoutingAutoBundle

Path not Exists Actions
-----------------------

These are the default actions available to take if the path provided by a
``path_provider`` does not exist.

create
~~~~~~

The ``create`` action will create the path.

.. configuration-block::

    .. code-block:: yaml

        not_exists_action: create

    .. code-block:: xml

        <not-exists-action name="create" />

    .. code-block:: php

        array(
            // ...
            'not_exists_action' => 'create',
        );

.. note::

    **Currently** all routes provided by the content path builder units will be
    created as ``Generic`` documents, whilst the content name route will be
    created as an ``AutoRoute`` document.

throw_exception
~~~~~~~~~~~~~~~

This action will throw an exception if the route provided by the path provider
does not exist. You should take this action if you are sure that the route
*should* exist.

.. configuration-block::

    .. code-block:: yaml

        not_exists_action: throw_exception

    .. code-block:: xml

        <not-exists-action name="throw_exception" />

    .. code-block:: php

        array(
            // ...
            'not_exists_action' => 'throw_exception',
        );
