Doctrine Caching for Jackalope Doctrine DBAL
============================================

Jackalope Doctrine DBAL gets better performance when running it with the
`DoctrineCacheBundle`_:

.. code-block:: bash

    $ php composer.phar require doctrine/cache-bundle:1.0.*

And adding the following entry to your ``app/AppKernel.php``::

    // app/AppKernel.php

    // ...
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Doctrine\Bundle\DoctrineCacheBundle\DoctrineCacheBundle(),
        );

        // ...
    }

Configure a ``doctrine_cache`` section in your main configuration file and add
a caches section to ``doctrine_phpcr.session.backend``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_cache:
            providers:
                phpcr_meta:
                    type: file_system
                phpcr_nodes:
                    type: file_system

        doctrine_phpcr:
            session:
                backend:
                    # ...
                    caches:
                        meta: doctrine_cache.providers.phpcr_meta
                        nodes: doctrine_cache.providers.phpcr_nodes

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://doctrine-project.org/schema">
                <provider name="phpcr_meta" type="file_system"/>
                <provider name="phpcr_nodes" type="file_system"/>
            </config>
            <config xmlns="http://doctrine-project.org/schema/symfony-dic/odm/phpcr">
                <session>
                    <backend>
                        <!-- ... -->
                        <caches
                            meta="doctrine_cache.providers.phpcr_meta"
                            nodes="doctrine_cache.providers.phpcr_nodes"
                        >
                    </backend>
                </session>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine_cache', array(
            'providers' => array(
                'phpcr_meta' => array(
                    'type' => 'file_system',
                ),
                'phpcr_nodes' => array(
                    'type' => 'file_system',
                ),
            ),
        );

        $container->loadFromExtension('doctrine_phpcr', array(
            'session' => array(
                'backend' => array(
                    // ...
                    'caches' => array(
                        'meta' => 'doctrine_cache.providers.phpcr_meta',
                        'nodes' => 'doctrine_cache.providers.phpcr_nodes',
                    ),
                ),
            ),
        );

.. _`DoctrineCacheBundle`: https://github.com/doctrine/DoctrineCacheBundle/
