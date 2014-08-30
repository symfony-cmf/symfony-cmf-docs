Configuration Reference
=======================

The BlockBundle can be configured under the ``cmf_block`` key in your
application configuration. When using XML, you can use the
``http://cmf.symfony.com/schema/dic/block`` namespace.

The BlockBundle *automatically* changes some defaults and adds configuration
to the SonataBlockBundle to make the integration work seamlessly. See the
:ref:`updated SonataBlockBundle defaults <bundle-block-updated-sonata-defaults>`
for more information.

Configuration
-------------

.. _reference-config-block-persistence:

persistence
~~~~~~~~~~~

phpcr
.....

This defines the persistence driver. The default configuration of persistence
is the following configuration:

.. configuration-block::

    .. code-block:: yaml

        cmf_block:
            persistence:
                phpcr:
                    enabled:                  false
                    block_basepath:           /cms/content
                    manager_name:             ~
                    string_document_class:    Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\StringBlock
                    simple_document_class:    Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\SimpleBlock
                    container_document_class: Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ContainerBlock
                    reference_document_class: Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ReferenceBlock
                    action_document_class:    Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ActionBlock
                    slideshow_document_class: Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\SlideshowBlock
                    imagine_document_class:   Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ImagineBlock
                    use_sonata_admin:         auto
                    string_admin_class:       Symfony\Cmf\Bundle\BlockBundle\Admin\StringBlockAdmin
                    simple_admin_class:       Symfony\Cmf\Bundle\BlockBundle\Admin\SimpleBlockAdmin
                    container_admin_class:    Symfony\Cmf\Bundle\BlockBundle\Admin\ContainerBlockAdmin
                    reference_admin_class:    Symfony\Cmf\Bundle\BlockBundle\Admin\ReferenceBlockAdmin
                    action_admin_class:       Symfony\Cmf\Bundle\BlockBundle\Admin\ActionBlockAdmin
                    slideshow_admin_class:    Symfony\Cmf\Bundle\BlockBundle\Admin\Imagine\SlideshowBlockAdmin
                    imagine_admin_class:      Symfony\Cmf\Bundle\BlockBundle\Admin\Imagine\ImagineBlockAdmin

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/block">
                <persistence>
                    <phpcr
                        enabled="false"
                        manager-name="null"
                        block-basepath="/cms/content"
                        string-document-class="Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\StringBlock"
                        simple-document-class="Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\SimpleBlock"
                        container-document-class="Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ContainerBlock"
                        reference-document-class="Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ReferenceBlock"
                        action-document-class="Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ActionBlock"
                        slideshow-document-class="Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\SlideshowBlock"
                        imagine-document-class="Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ImagineBlock"
                        use-sonata-admin="auto"
                        string-admin-class="Symfony\Cmf\Bundle\BlockBundle\Admin\StringBlockAdmin"
                        simple-admin-class="Symfony\Cmf\Bundle\BlockBundle\Admin\SimpleBlockAdmin"
                        container-admin-class="Symfony\Cmf\Bundle\BlockBundle\Admin\ContainerBlockAdmin"
                        reference-admin-class="Symfony\Cmf\Bundle\BlockBundle\Admin\ReferenceBlockAdmin"
                        action-admin-class="Symfony\Cmf\Bundle\BlockBundle\Admin\ActionBlockAdmin"
                        slideshow-admin-class="Symfony\Cmf\Bundle\BlockBundle\Admin\Imagine\SlideshowBlockAdmin"
                        imagine-admin-class="Symfony\Cmf\Bundle\BlockBundle\Admin\Imagine\ImagineBlockAdmin"
                    />
                </persistence>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_block', array(
            'persistence' => array(
                'phpcr' => array(
                    'enabled'                  => false,
                    'block-basepath'           => '/cms/content',
                    'manager_name'             => null,
                    'string_document_class'    => 'Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\StringBlock',
                    'simple_document_class'    => 'Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\SimpleBlock',
                    'container_document_class' => 'Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ContainerBlock',
                    'reference_document_class' => 'Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ReferenceBlock',
                    'action_document_class'    => 'Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ActionBlock',
                    'slideshow_document_class' => 'Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\SlideshowBlock',
                    'imagine_document_class'   => 'Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ImagineBlock',
                    'use_sonata_admin'         => 'auto',
                    'string_admin_class'       => 'Symfony\Cmf\Bundle\BlockBundle\Admin\StringBlockAdmin',
                    'simple_admin_class'       => 'Symfony\Cmf\Bundle\BlockBundle\Admin\SimpleBlockAdmin',
                    'container_admin_class'    => 'Symfony\Cmf\Bundle\BlockBundle\Admin\ContainerBlockAdmin',
                    'reference_admin_class'    => 'Symfony\Cmf\Bundle\BlockBundle\Admin\ReferenceBlockAdmin',
                    'action_admin_class'       => 'Symfony\Cmf\Bundle\BlockBundle\Admin\ActionBlockAdmin',
                    'slideshow_admin_class'    => 'Symfony\Cmf\Bundle\BlockBundle\Admin\Imagine\SlideshowBlockAdmin',
                    'imagine_admin_class'      => 'Symfony\Cmf\Bundle\BlockBundle\Admin\Imagine\ImagineBlockAdmin',
                ),
            ),
        ));


enabled
"""""""

.. include:: ../_partials/persistence_phpcr_enabled.rst.inc

block_basepath
""""""""""""""

**type**: ``string`` **default**: ``/cms/content``

The basepath for blocks in the PHPCR tree.

If the :doc:`CoreBundle <../core/introduction>` is registered, this will default to
the value of ``%cmf_core.persistence.phpcr.basepath%/content``.

manager_name
""""""""""""

.. include:: ../_partials/persistence_phpcr_manager_name.rst.inc

string_document_class
"""""""""""""""""""""

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\StringBlock``

The string block document class.

simple_document_class
"""""""""""""""""""""

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\SimpleBlock``

The simple block document class.

container_document_class
""""""""""""""""""""""""

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ContainerBlock``

The container block document class.

reference_document_class
""""""""""""""""""""""""

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ReferenceBlock``

The reference block document class.

action_document_class
"""""""""""""""""""""

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ActionBlock``

The action block document class.

If phpcr is enabled ``use_sonata_admin`` is enabled, the class value is set in
``Resources/config/admin.xml``.

slideshow_document_class
""""""""""""""""""""""""

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\SlideshowBlock``

The slideshow block document class.

imagine_document_class
""""""""""""""""""""""

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ImagineBlock``

The imagine block document class. This document will only work properly if
you set up the LiipImagineBundle.

use_sonata_admin
""""""""""""""""

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

If ``true``, the admin classes are activated. If set to ``auto``, they are
activated only if the SonataPhpcrAdminBundle is present.

If the :doc:`CoreBundle <../core/introduction>` is registered, this will default to the value
of ``cmf_core.persistence.phpcr.use_sonata_admin``.

string_admin_class
""""""""""""""""""

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\BlockBundle\Admin\StringBlockAdmin``

The sonata admin class of the string block.

simple_admin_class
""""""""""""""""""

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\BlockBundle\Admin\SimpleBlockAdmin``

The sonata admin class of the simple block.

container_admin_class
"""""""""""""""""""""

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\BlockBundle\Admin\ContainerBlockAdmin``

The sonata admin class of the container block.

reference_admin_class
"""""""""""""""""""""

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\BlockBundle\Admin\ReferenceBlockAdmin``

The sonata admin class of the reference block.

action_admin_class
""""""""""""""""""

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\BlockBundle\Admin\ActionBlockAdmin``

The sonata admin class of the action block.

slideshow_admin_class
"""""""""""""""""""""

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\BlockBundle\Admin\Imagine\SlideshowBlockAdmin``

The sonata admin class of the slideshow block.

This admin will only be loaded if ``use_imagine`` is enabled.

imagine_admin_class
"""""""""""""""""""

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\BlockBundle\Admin\Imagine\ImagineBlockAdmin``

The sonata admin class of the imagine block.

This admin will only be loaded if ``use_imagine`` is enabled.

Twig
~~~~

.. _reference-config-block-twig-cmf-embed-blocks:

cmf_embed_blocks
................

The BlockBundle provides a Twig filter ``cmf_embed_blocks`` that
looks through the content and looks for special tags to render blocks.

See :ref:`embed blocks in content <bundle-block-embed>` for using the
``cmf_embed_blocks`` filter.

prefix
""""""

**type**: ``string`` **default**: ``%embed-block|``

The part before the actual path to the block.

postfix
"""""""

**type**: ``string`` **default**: ``|end%``

The part after the actual path to the block.

use_imagine
~~~~~~~~~~~

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

If ``true``, the imagine related block classes and admin classes are activated.
If set to ``auto``, they are activated only if the LiipImagineBundle is present.

caches
~~~~~~

The BlockBundle integrates with the `SonataCacheBundle`_ to provide several
caching solutions.

.. _reference-config-block-caches-esi:

varnish
.......

This extends the default VarnishCache adapter of the SonataCacheBundle.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            esi: { enabled: true }
            # enable FragmentListener to automatically validate and secure fragments
            fragments: { path: /_fragment }
            # add varnish server ip-address(es)
            trusted_proxies: [192.0.0.1, 10.0.0.0/8]

        cmf_block:
            # ...
            caches:
                varnish:
                    token: a unique security key # a random one is generated by default
                    servers:
                        - varnishadm -T 127.0.0.1:2000 {{ COMMAND }} "{{ EXPRESSION }}"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/block">
                <caches>
                    <!-- token: a random one is generated by default -->
                    <varnish token="a unique security key">
                        <server>varnishadm -T 127.0.0.1:2000 {{ COMMAND }} "{{ EXPRESSION }}"</server>
                    </varnish>
                </caches>
            </config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_block', array(
            // ...
            'caches' => array(
                'varnish' => array(
                    'token' => 'a unique security key', // a random one is generated by default
                    'servers' => array(
                        'varnishadm -T 127.0.0.1:2000 {{ COMMAND }} "{{ EXPRESSION }}"',
                    ),
                ),
            ),
        ));

token
"""""

**type**: ``string`` **default**: ``hash('sha256', uniqid(mt_rand(), true))``

A unique secret key. A random one is generated by default.

servers
"""""""

**type**: ``array``

.. _reference-config-block-caches-ssi:

ssi
...

This extends the default SsiCache adapter of the SonataCacheBundle.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_block:
            # ...
            caches:
                ssi:
                   token: a unique security key # a random one is generated by default

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/block">
                <caches>
                    <!-- token: a random one is generated by default -->
                    <ssi
                        token="a unique security key"
                    />
                </caches>
            </config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_block', array(
            // ...
            'caches' => array(
                'ssi' => array(
                    'token' => 'a unique security key', // a random one is generated by default
                ),
            ),
        ));

token
"""""

**type**: ``string`` **default**: ``hash('sha256', uniqid(mt_rand(), true))``

A unique secret key. A random one is generated by default.

.. _`SonataCacheBundle`: https://github.com/sonata-project/SonataCacheBundle

