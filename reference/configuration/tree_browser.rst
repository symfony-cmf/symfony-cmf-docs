TreeBrowserBundle Configuration
===============================

The TreeBrowserBundle provides a tree navigation on top of a PHPCR
repository and can be configured under the ``cmf_tree_browser`` key in your
application configuration. When using XML, you can use the
``http://cmf.symfony.com/schema/dic/treebrowser`` namespace.

Configuration
-------------

.. _config-tree_browser-persistence:

persistence
~~~~~~~~~~~

phpcr
.....

This defines the persistence driver. The default configuration of persistence
is the following configuration:

.. configuration-block::

    .. code-block:: yaml

        cmf_tree_browser:
            persistence:
                phpcr:
                    enabled: false
                    session_name: default

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/treebrowser">
                <persistence>
                    <phpcr
                        enabled="false"
                        session-name="default"
                    />
                </persistence>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_tree_browser', array(
            'persistence' => array(
                'phpcr' => array(
                    'enabled' => false,
                    'session_name' => 'default',
                ),
            ),
        ));


enabled
"""""""

**type**: ``boolean`` **default**: ``false``

If ``true``, PHPCR is enabled in the service container.

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to
the value of ``cmf_core.persistence.phpcr.enabled``.

PHPCR can be enabled by multiple ways such as:

.. configuration-block::

    .. code-block:: yaml

        phpcr: ~ # use default configuration
        # or
        phpcr: true # straight way
        # or
        phpcr:
            manager: ... # or any other option under 'phpcr'

    .. code-block:: xml

        <persistence>
            <!-- use default configuration -->
            <phpcr />

            <!-- or setting it the straight way -->
            <phpcr>true</phpcr>

            <!-- or setting an option under 'phpcr' -->
            <phpcr manager="..." />
        </persistence>

    .. code-block:: php

        $container->loadFromExtension('cmf_simple_cms', array(
            // ...
            'persistence' => array(
                'phpcr' => null, // use default configuration
                // or
                'phpcr' => true, // straight way
                // or
                'phpcr' => array(
                    'manager' => '...', // or any other option under 'phpcr'
                ),
            ),
        ));

session_name
""""""""""""

**type**: ``string`` **default**: ``default``

The name of the connection.
