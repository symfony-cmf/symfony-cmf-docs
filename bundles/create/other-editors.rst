Other Editors
=============

    You can use the lightweight hallo.js editor with CreateBundle or write
    your own template to load a different editor or customize one of the
    existing editors is configured.

.. _bundle-create-hallo:

Using Hallo.js Editor
---------------------

If you want to use the hallo.js editor instead of CKEditor, you should
configure the ``downloadCreate`` instead of ``downloadCreateAndCkeditor``
composer script handler. The files for hallo.js are already included in the
create.js distribution:

.. code-block:: javascript

    {
        "scripts": {
            "post-install-cmd": [
                "Symfony\\Cmf\\Bundle\\CreateBundle\\Composer\\ScriptHandler::downloadCreate",
                ...
            ],
            "post-update-cmd": [
                "Symfony\\Cmf\\Bundle\\CreateBundle\\Composer\\ScriptHandler::downloadCreate",
                ...
            ]
        }
    }

Then re-run composer:

.. code-block:: bash

    $ composer run-scripts

In your template, load the JavaScript files using:

.. configuration-block::

    .. code-block:: jinja

        {% render(controller("cmf_create.jsloader.controller:includeJSFilesAction")) %}

    .. code-block:: php

        <?php $view['actions']->render(
            'cmf_create.jsloader.controller:includeJSFilesAction',
            array(
                'editor' => 'hallo',
            )
        ) ?>

If you want to use the image plugin of hallo, you also need to register
the additional image controller routing file in your main routing configuration:

.. configuration-block::

    .. code-block:: yaml

        create_image:
            resource: "@CmfCreateBundle/Resources/config/routing/image.xml"

    .. code-block:: xml

        <import resource="@CmfCreateBundle/Resources/config/routing/image.xml" />

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@CmfCreateBundle/Resources/config/routing/image.xml"));

        return $collection;

.. _bundle-create-custom:

Custom Editors
--------------

You can provide your own template to customize how to load CKEditor, hallo.js
or a WYSIWYG editor not supported out of the box. The template has
follow the naming pattern
``CmfCreateBundle::includejsfiles-%editor%.html.twig`` to be loaded. You custom
file thus needs to reside in ``app/Resources/CmfCreateBundle/views/`` and has
to be called ``includejsfiles-myeditor.html.twig`` where ``myeditor`` is the name
you want to use in the action. In the ``includeJSFilesAction``, you specify the
editor parameter:

.. configuration-block::

    .. code-block:: jinja

        {% render(controller(
                "cmf_create.jsloader.controller:includeJSFilesAction",
                 {'editor': 'myeditor' }
        )) %}

    .. code-block:: php

        <?php $view['actions']->render(
            new ControllerReference('cmf_create.jsloader.controller:includeJSFilesAction', array(
                'editor'  => 'myeditor',
            ))
        ); ?>

.. note::

    Create.js has built in support for the `Aloha editor`_ as well. We hope to
    provide out of the box support for Aloha in this bundle too. If you want to
    help, please see the github issue for `aloha`_ integration.

.. _`Aloha editor`: http://www.aloha-editor.org/
.. _`aloha`: https://github.com/symfony-cmf/CreateBundle/issues/32
