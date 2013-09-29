BlockBundle Configuration
=========================

The BlockBundle provides integration with SonataBlockBundle and can be
configured under the ``cmf_block`` key in your application configuration. When
using XML, you can use the ``http://cmf.symfony.com/schema/dic/block``
namespace.

Configuration
-------------

.. _config-block-persistence:

persistence
~~~~~~~~~~~

phpcr
.....

This defines the persistence driver. The default configuration of persistence
is the following configuration:

.. configuration-block::

    .. code-block:: yaml

        cmf_simple_cms:
            persistence:
                phpcr:
                    enabled: false
                    manager_name: ~
                    block_basepath: /cms/content
                    simple_document_class: ~
                    container_document_class: ~
                    reference_document_class: ~
                    action_document_class: ~
                    slideshow_document_class: ~
                    imagine_document_class: ~
                    simple_admin_class: ~
                    container_admin_class: ~
                    reference_admin_class: ~
                    action_admin_class: ~
                    slideshow_admin_class: ~
                    imagine_admin_class: ~
                    use_sonata_admin: auto

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/block">
                <persistence>
                    <phpcr
                        enabled="false"
                        manager-name="null"
                        block-basepath="/cms/content"
                        simple-document-class="null"
                        container-document-class="null"
                        reference-document-class="null"
                        action-document-class="null"
                        slideshow-document-class="null"
                        imagine-document-class="null"
                        simple-admin-class="null"
                        container-admin-class="null"
                        reference-admin-class="null"
                        action-admin-class="null"
                        slideshow-admin-class="null"
                        imagine-admin-class="null"
                        use-sonata-admin="auto"
                    />
                </persistence>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_simple_cms', array(
            'persistence' => array(
                'phpcr' => array(
                    'enabled' => false,
                    'block-basepath' => '/cms/block',
                    'manager_name' => null,
                    'simple_document_class' => null,
                    'container_document_class' => null,
                    'reference_document_class' => null,
                    'action_document_class' => null,
                    'slideshow_document_class' => null,
                    'imagine_document_class' => null,
                    'simple_admin_class' => null,
                    'container_admin_class' => null,
                    'reference_admin_class' => null,
                    'action_admin_class' => null,
                    'slideshow_admin_class' => null,
                    'imagine_admin_class' => null,
                    'use_sonata_admin' => 'auto',
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
            manager_name: ... # or any other option under 'phpcr'

    .. code-block:: xml

        <persistence>
            <!-- use default configuration -->
            <phpcr />

            <!-- or setting it the straight way -->
            <phpcr>true</phpcr>

            <!-- or setting an option under 'phpcr' -->
            <phpcr manager-name="..." />
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
                    'manager_name' => '...', // or any other option under 'phpcr'
                ),
            ),
        ));

basepath
""""""""

**type**: ``string`` **default**: ``/cms/simple``

The basepath for CMS documents in the PHPCR tree.

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to
the value of ``%cmf_core.persistence.phpcr.basepath%/simple``.

manager_name
""""""""""""

**type**: ``string`` **default**: ``null``

The name of the Doctrine Manager to use. ``null`` tells the manager registry to
retrieve the default manager.<persistence>

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to
the value of ``cmf_core.persistence.phpcr.manager_name``.

simple_document_class
"""""""""""""""""""""

**type**: ``string`` **default**: ``null``

container_document_class
""""""""""""""""""""""""

**type**: ``string`` **default**: ``null``

reference_document_class
""""""""""""""""""""""""

**type**: ``string`` **default**: ``null``

action_document_class
"""""""""""""""""""""

**type**: ``string`` **default**: ``null``

slideshow_document_class
""""""""""""""""""""""""

**type**: ``string`` **default**: ``null``

imagine_document_class
""""""""""""""""""""""

**type**: ``string`` **default**: ``null``

simple_admin_class
""""""""""""""""""

**type**: ``string`` **default**: ``null``

container_admin_class
"""""""""""""""""""""

**type**: ``string`` **default**: ``null``

reference_admin_class
"""""""""""""""""""""

**type**: ``string`` **default**: ``null``

action_admin_class
""""""""""""""""""

**type**: ``string`` **default**: ``null``

slideshow_admin_class
"""""""""""""""""""""

**type**: ``string`` **default**: ``null``

imagine_admin_class
"""""""""""""""""""

**type**: ``string`` **default**: ``null``

use_sonata_admin
""""""""""""""""

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

twig
~~~~

cmf_embed_blocks
................

prefix
""""""

**type**: ``string`` **default**: ``%embed-block|``

postfix
"""""""

**type**: ``string`` **default**: ``|end%``

use_imagine
~~~~~~~~~~~

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``
