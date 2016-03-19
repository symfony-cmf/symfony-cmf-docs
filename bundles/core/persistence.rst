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

        cmf_core:
            persistence:
                phpcr: ~

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/core">
                <persistence>
                    <phpcr />
                </persistence>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_core', array(
            'persistence' => array(
                'phpcr' => array(),
            ),
        ));

.. _bundles-core-multilang-persisting_multilang_documents:

Persisting Documents in Different Languages
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Refer to the `PHPCR-ODM documentation`_ for details on persisting documents in different languages.

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

        cmf_core:
            persistence:
                phpcr:
                    translation_strategy: attribute

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/core">
                <persistence>
                    <phpcr
                        translation-strategy="attribute"
                    />
                </persistence>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_core', array(
            'persistence' => array(
                'phpcr' => array(
                    'translation_strategy' => 'attribute',
                ),
            ),
        ));

.. caution::

    Changing this setting when data was already persisted with a different
    translation strategy will require manually updating the current data
    to match that of the chosen translation strategy.

See the `PHPCR-ODM documentation`_ for more information.

.. _bundle-core-child-admin-extension:

Using Child Models: The Child Sonata Admin Extension
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This extension sets a default parent to every new
object instance if a ``parent`` parameter is present in the URL.
The parent parameter is present for example when adding documents
in an overlay with the ``doctrine_phpcr_odm_tree_manager``
or when adding a document in the tree of the dashboard.

.. note::

    This extension is only available if ``cmf_core.persistence.phpcr`` is enabled
    and SonataPHPCRAdminBundle is active.

To enable the extension in your admin classes, simply define the extension
configuration in the ``sonata_admin`` section of your project configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            # ...
            extensions:
                cmf_core.admin_extension.child:
                    implements:
                        - Symfony\Cmf\Bundle\CoreBundle\Model\ChildInterface
                        - Doctrine\ODM\PHPCR\HierarchyInterface

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://sonata-project.org/schema/dic/admin">
                <!-- ... -->
                <extension id="cmf_core.admin_extension.child">
                    <implement>Symfony\Cmf\Bundle\CoreBundle\Model\ChildInterface</implement>
                    <implement>Doctrine\ODM\PHPCR\HierarchyInterface</implement>
                </extension>
            </config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_admin', array(
            // ...
            'extensions' => array(
                'cmf_core.admin_extension.child' => array(
                    'implements' => array(
                        'Symfony\Cmf\Bundle\CoreBundle\Model\ChildInterface',
                        'Doctrine\ODM\PHPCR\HierarchyInterface',
                    ),
                ),
            ),
        ));

See the `Sonata Admin extension documentation`_ for more information.

.. _bundle-core-translatable-admin-extension:

Editing Locale Information: Translatable Sonata Admin Extension
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Several bundles provide translatable model classes that implement
``TranslatableInterface``. This extension adds a locale field
to the given SonataAdminBundle forms.

To enable the extensions in your admin classes, simply define the extension
configuration in the ``sonata_admin`` section of your project configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            # ...
            extensions:
                cmf_core.admin_extension.translatable:
                    implements:
                        - Symfony\Cmf\Bundle\CoreBundle\Translatable\TranslatableInterface

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://sonata-project.org/schema/dic/admin">
                <!-- ... -->
                <extension id="cmf_core.admin_extension.translatable">
                    <implement>
                        Symfony\Cmf\Bundle\CoreBundle\Translatable\TranslatableInterface
                    </implement>
                </extension>
            </config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_admin', array(
            // ...
            'extensions' => array(
                'cmf_core.admin_extension.translatable' => array(
                    'implements' => array(
                        'Symfony\Cmf\Bundle\CoreBundle\Translatable\TranslatableInterface',
                    ),
                ),
            ),
        ));

See the `Sonata Admin extension documentation`_ for more information.

.. _`Sonata Admin extension documentation`: https://sonata-project.org/bundles/admin/master/doc/reference/extensions.html
.. _`PHPCR-ODM documentation`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/multilang.html#full-example
