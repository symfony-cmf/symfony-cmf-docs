.. index::
    single: Multi-Language; CoreBundle


Multi-language support
----------------------

.. _bundles-core-multilang-persisting_multilang_documents:

Persisting Documents in Different Languages
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Refer to the `PHPCR-ODM documentation`_ for details on persisting documents in different languages.

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

.. _`Sonata Admin extension documentation`: http://sonata-project.org/bundles/admin/master/doc/reference/extensions.html
.. _`PHPCR-ODM documentation`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/multilang.html#full-example
