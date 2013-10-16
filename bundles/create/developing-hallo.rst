Developing the Hallo Wysiwyg Editor
===================================

    You can develop the hallo editor inside the Create bundle. By default, a
    minimized version of hallo that is bundled with create is used. To develop
    the actual code, you will need to checkout the full hallo repository first.
    You can do this by running the following command from the command line:

.. code-block:: bash

    $ php app/console cmf:create:init-hallo-devel

There is a special template to load the coffee script files. To load this,
just use the ``hallo-coffee`` editor with the includeJSFilesAction.

.. configuration-block::

    .. code-block:: jinja

        {% render controller("cmf_create.jsloader.controller:includeJSFilesAction" with {'editor': 'hallo-coffee', '_locale': app.request.locale }) %}

    .. code-block:: php

        <?php $view['actions']->render(
            new ControllerReference('cmf_create.jsloader.controller:includeJSFilesAction", array(
                'editor'  => 'hallo-coffee',
                '_locale' => $app->getRequest()->getLocale(),
            ))
        ) ?>

The hallo-coffee template uses assetic to load the coffee script files from
``Resources/public/vendor/hallo/src``, rather than the precompiled javascript
from ``Resources/public/vendor/create/deps/hallo-min.js``. This also means
that you need to add a mapping for coffeescript in your assetic configuration
and you need the `coffee compiler set up correctly`_.

.. configuration-block::

    .. code-block:: yaml

        assetic:
            filters:
                cssrewrite: ~
                coffee:
                    bin: %coffee.bin%
                    node: %coffee.node%
                    apply_to: %coffee.extension%

    .. code-block:: xml

        <config xmlns="http://symfony.com/schema/dic/assetic">
            <filter name="cssrewite" />
            <filter name="coffee"
                bin="%coffee.bin%"
                node="%coffee.node%"
                apply-to="%coffee.extension%" />
        </config>

    .. code-block:: php

        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'cssrewrite' => null,
                'coffee'     => array(
                    'bin'      => '%coffee.bin%',
                    'node'     => '%coffee.node%',
                    'apply_to' => '%coffee.extension%',
                ),
            ),
        ));

In the cmf-sandbox we did a little hack to not always trigger coffee script
compiling.  In config.yml we make the coffee extension configurable. Now if
the parameters.yml sets ``coffee.extension`` to ``\.coffee`` the coffeescript
is compiled and the coffee compiler needs to be installed. If you set it to
anything else like ``\.nocoffee`` then you do not need the coffee compiler
installed as no file has the extension ``\.nocoffee``.

The default values for the three parameters are

.. code-block:: yaml

    # app/config/parameters.yml

    # ...
    coffee.bin: /usr/local/bin/coffee
    coffee.node: /usr/local/bin/node
    coffee.extension: \.coffee

.. _`coffee compiler set up correctly`: http://coffeescript.org/#installation