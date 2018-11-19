.. index::
    single: Multi-Language; CoreBundle

.. _bundles-core-persistence:

Persistence
-----------

The CoreBundle lets you to centrally configure the persistence layer for all
CMF bundles.

To make the PHPCR-ODM the default persistence layer for all CMF bundles add
the following to your main configuration file:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_core:
            persistence:
                phpcr: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/core">
                <persistence>
                    <phpcr />
                </persistence>
            </config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_core', [
            'persistence' => [
                'phpcr' => [],
            ],
        ]);

.. _bundles-core-multilang-persisting_multilang_documents:

Persisting Documents in Different Languages
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Refer to the `PHPCR-ODM documentation`_ for details on persisting documents in different languages.

.. _bundles-core-multilang-global_translation_strategy:

Choosing a Global Translation Strategy
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

PHPCR-ODM supports multiple different strategies for persisting translations in the
repository. When combining Bundles its possible that one ends up with a mix of
different strategies which can make providing a generic search across this data
more complicated and might also be less efficient depending on the number of
different languages used in the system.

For this purpose the CoreBundle provides a Doctrine listener that can optionally
enforce a single translation strategy for all documents:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_core:
            persistence:
                phpcr:
                    translation_strategy: attribute

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/core">
                <persistence>
                    <phpcr translation-strategy="attribute"/>
                </persistence>
            </config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_core', [
            'persistence' => [
                'phpcr' => [
                    'translation_strategy' => 'attribute',
                ],
            ],
        ]);

.. caution::

    Changing this setting when data was already persisted with a different
    translation strategy will require manually updating the current data
    to match that of the chosen translation strategy.

See the `PHPCR-ODM documentation`_ for more information.

.. _`PHPCR-ODM documentation`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/multilang.html#full-example
