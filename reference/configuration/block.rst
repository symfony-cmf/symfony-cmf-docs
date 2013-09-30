BlockBundle Configuration
=========================

The BlockBundle provides integration with SonataBlockBundle and can be
configured under the ``cmf_block`` key in your application configuration. When
using XML, you can use the ``http://cmf.symfony.com/schema/dic/block``
namespace.

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

        $container->loadFromExtension('cmf_block', array(
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

        $container->loadFromExtension('cmf_block', array(
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

block_basepath
""""""""""""""

**type**: ``string`` **default**: ``/cms/content``

The basepath for blocks in the PHPCR tree.

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to
the value of ``%cmf_core.persistence.phpcr.basepath%/content``.

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

The simple block document class.

If phpcr is enabled ``use_sonata_admin`` is enabled, the class value is set in
``Resources/config/admin.xml``.

container_document_class
""""""""""""""""""""""""

**type**: ``string`` **default**: ``null``

The container block document class.

If phpcr is enabled ``use_sonata_admin`` is enabled, the class value is set in
``Resources/config/admin.xml``.

reference_document_class
""""""""""""""""""""""""

**type**: ``string`` **default**: ``null``

The reference block document class.

If phpcr is enabled ``use_sonata_admin`` is enabled, the class value is set in
``Resources/config/admin.xml``.

action_document_class
"""""""""""""""""""""

**type**: ``string`` **default**: ``null``

The action block document class.

If phpcr is enabled ``use_sonata_admin`` is enabled, the class value is set in
``Resources/config/admin.xml``.

slideshow_document_class
""""""""""""""""""""""""

**type**: ``string`` **default**: ``null``

The slideshow block document class.

If phpcr is enabled and ``use_imagine`` is enabled, the class value is set in
``Resources/config/admin-imagine.xml``.

imagine_document_class
""""""""""""""""""""""

**type**: ``string`` **default**: ``null``

The imagine block document class.

If phpcr is enabled, ``use_sonata_admin`` is enabled and ``use_imagine`` is
enabled, the class value is set in ``Resources/config/admin-imagine.xml``.

simple_admin_class
""""""""""""""""""

**type**: ``string`` **default**: ``null``

The sonata admin class of the simple block.

If phpcr is enabled and ``use_sonata_admin`` is enabled, the class value is set
in ``Resources/config/admin.xml``.

container_admin_class
"""""""""""""""""""""

**type**: ``string`` **default**: ``null``

The sonata admin class of the container block.

If phpcr is enabled and ``use_sonata_admin`` is enabled, the class value is set
in ``Resources/config/admin.xml``.

reference_admin_class
"""""""""""""""""""""

**type**: ``string`` **default**: ``null``

The sonata admin class of the reference block.

If phpcr is enabled and ``use_sonata_admin`` is enabled, the class value is set
in ``Resources/config/admin.xml``.

action_admin_class
""""""""""""""""""

**type**: ``string`` **default**: ``null``

The sonata admin class of the action block.

If phpcr is enabled and ``use_sonata_admin`` is enabled, the class value is set
in ``Resources/config/admin.xml``.

slideshow_admin_class
"""""""""""""""""""""

**type**: ``string`` **default**: ``null``

The sonata admin class of the slideshow block.

If phpcr is enabled, ``use_sonata_admin`` is enabled and ``use_imagine`` is
enabled, the class value is set in ``Resources/config/admin-imagine.xml``.

imagine_admin_class
"""""""""""""""""""

**type**: ``string`` **default**: ``null``

The sonata admin class of the imagine block.

If phpcr is enabled, ``use_sonata_admin`` is enabled and ``use_imagine`` is
enabled, the class value is set in ``Resources/config/admin-imagine.xml``.

use_sonata_admin
""""""""""""""""

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

If ``true``, the block classes and admin classes are activated. If set to
``auto``, they are activated only if the SonataPhpcrAdminBundle is present.

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to the value
of ``cmf_core.persistence.phpcr.use_sonata_admin``.

twig
~~~~

.. _reference-config-block-twig-cmf-embed-blocks:

cmf_embed_blocks
................

The BlockBundle provides a twig filter ``cmf_embed_blocks`` that
looks through the content and looks for special tags to render blocks.

See :ref:`embed blocks in content <tutorial-block-embed>` for using the
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
