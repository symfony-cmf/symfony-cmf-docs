Configuration Reference
=======================

The CreateBundle can be configured under the ``cmf_create`` key in your
application configuration. When using XML you should use the
``http://cmf.symfony.com/schema/dic/create`` namespace.

.. note::

    The CreateBundle provides no model classes of its own. It still needs to
    know what persistence layer you are using, in order to decide what service
    to use to interact with the storage layer during save operations.

Configuration
-------------

Security
~~~~~~~~

The controller that receives save requests from create.js does a security check
to determine whether the current user is allowed to edit content. As it would
not be convenient to show the create.js editor to users not allowed to edit the
site, the controller loading the create.js JavaScript files with the
``includeJSFilesAction`` also uses the same security check, as does the image
upload controller if it is activated.

The default security check checks if the user has a specified role. If nothing
is configured, the default role is ``ROLE_ADMIN``. If you set the parameter to
boolean ``false``, every user will be allowed to save changes through the REST
controller.

A last option is to configure your own ``checker_service`` to be used instead
of the role based check.

For more information, see the
:ref:`security section in the bundle doc <bundle_create_introduction_access_control>`.

.. configuration-block::

    .. code-block:: yaml

        cmf_create:
            security:
                role:            ROLE_ADMIN
                checker_service: ~

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/create">
                <security role="ROLE_ADMIN" checker-service="null" />
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_create', array(
            'security' => array(
                'role'            => 'ROLE_ADMIN',
                'checker_service' => null,
            ),
        ));

.. _config-create-object-mapper-service-id:

``object_mapper_service_id``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``"cmf_create.chain_mapper"``

You can specify a service implementing ``Midgard\CreatePHP\RdfMapperInterface``
that will handle objects that need to be stored by the REST handler of
CreatePHP. You need to either specify this service, enable phpcr or orm
persistence or define one or more services that implement the
``Midgard\CreatePHP\RdfChainableMapperInterface`` and tag them with ``cmf_create.mapper``.

.. _config-create-persistence:

``persistence``
~~~~~~~~~~~~~~~

This defines a persistence driver for Doctrine PHPCR-ODM documents or Doctrine ORM entities.
If you specify neither, see :ref:`config-create-object-mapper-service-id`.

``phpcr``
.........

.. configuration-block::

    .. code-block:: yaml

        cmf_create:
            persistence:
                phpcr:
                    enabled:      false
                    manager_name: ~
                    image:
                        enabled:          false
                        model_class:      ~
                        controller_class: Symfony\Cmf\Bundle\CreateBundle\Controller\ImageController
                        basepath:         /cms/media
                    delete:               false

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/create">
                <persistence>
                    <phpcr
                        enabled="false"
                        manager-name="null"
                        delete="false"
                    >
                        <image
                            enabled="false"
                            model-class="null"
                            controller-class="Symfony\Cmf\Bundle\CreateBundle\Controller\ImageController"
                            basepath="/cms/media"
                        />
                    </phpcr>
                </persistence>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_create', array(
            'persistence' => array(
                'phpcr' => array(
                    'enabled'      => false,
                    'manager_name' => null,
                    'image' => array(
                        'enabled'          => false,
                        'model_class'      => null,
                        'controller_class' => 'Symfony\Cmf\Bundle\CreateBundle\Controller\ImageController',
                        'basepath'         => '/cms/media',
                    ),
                    'delete' => false,
                ),
            ),
        ));


``enabled``
"""""""""""

.. include:: ../_partials/persistence_phpcr_enabled.rst.inc

``manager_name``
""""""""""""""""

.. include:: ../_partials/persistence_phpcr_manager_name.rst.inc

``image``
"""""""""

These settings are only used with the optional hallo editor. The default
CKEditor uses the :doc:`ELfinder plugin <../media/adapters/elfinder>`
provided by the MediaBundle.

If you need different image handling, you can either overwrite
``model_class`` and/or the ``controller_class``.

``delete``
""""""""""

**type**: ``boolean`` **default**: ``false``

Set delete to true to enable the simple delete workflow. This allows to directly
delete content from the frontend. Be careful, there are no special checks once you confirm deletion
your content is gone.

``orm``
.......

.. configuration-block::

    .. code-block:: yaml

        cmf_create:
            persistence:
                orm:
                    enabled:      false
                    manager_name: ~

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/create">
                <persistence>
                    <orm
                        enabled="false"
                        manager-name="null"
                    />
                </persistence>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_create', array(
            'persistence' => array(
                'orm' => array(
                    'enabled'      => false,
                    'manager_name' => null,
                ),
            ),
        ));


``enabled``
"""""""""""

**type**: ``boolean`` **default**: ``false``

If ``true``, the ORM is included in the chain mapper.

``manager_name``
""""""""""""""""

**type**: ``string`` **default**: ``null``

The name of the Doctrine Manager to use.


Metadata Handling
~~~~~~~~~~~~~~~~~

.. configuration-block::

    .. code-block:: yaml

        cmf_create:
            auto_mapping: true
            rdf_config_dirs:
                - '%kernel.root_dir%/Resources/rdf-mappings'
            map:
                '<http://rdfs.org/sioc/ns#Post>': 'Symfony\Cmf\Bundle\ContentBundle\Doctrine\Phpcr\StaticContent'

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/create"
                auto-mapping="true"
            >
                <rdf-config-dir>%kernel.root_dir%/Resources/rdf-mappings</rdf-config-dir>
                <map name="'<http://rdfs.org/sioc/ns#Post>'">Symfony\Cmf\Bundle\ContentBundle\Doctrine\Phpcr\StaticContent</map>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_create', array(
            'auto_mapping'    => true,
            'rdf_config_dirs' => array('%kernel.root_dir%/Resources/rdf-mappings'),
            'map'             => array('<http://rdfs.org/sioc/ns#Post>' => 'Symfony\Cmf\Bundle\ContentBundle\Doctrine\Phpcr\StaticContent'),
        ));

``auto_mapping``
................

If not set to false, the CreateBundle will look for mapping files in every
bundle in the directory ``Resources/rdf-mappings``.

``rdf_config_dirs``
...................

Additional directories to look for mapping files besides the auto mapped
directory.

``map``
.......

Mapping from RDF type name to class. This configuration is used when adding
items to collections. *Note that collection support is currently incomplete
in the CreateBundle.*

REST handler
~~~~~~~~~~~~

You can configure the REST handler class with the ``rest_controller_class``
option. Furthermore it is possible to enable ``rest_force_request_locale``.
When this option is enabled, the current request locale is set on the model
instance. This is useful in order to automatically translate a model to
the request locale when using in-line editing, instead of editing the model
in the locale in which it is currently stored, which might be different
than the request locale due to language fallback.

.. note::

    The ``rest_force_request_locale`` option requires that the
    :doc:`CoreBundle <../core/introduction>` is enabled.

Editor configuration
~~~~~~~~~~~~~~~~~~~~

You can tweak a few things on the editor. Most of the time, the only relevant
setting is the ``plain_text_types``.

.. configuration-block::

    .. code-block:: yaml

        cmf_create:
            plain_text_types: ["dcterms:title"]
            editor_base_path: /bundles/cmfcreate/vendor/ckeditor/
            fixed_toolbar:    true
            stanbol_url:      http://dev.iks-project.eu:8081

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/create"
                editor-base-path="/bundles/cmfcreate/vendor/ckeditor/"
                fixed-toolbar="true"
                stanbol_url="http://dev.iks-project.eu:8081"
            >
                <plain-text-type>dcterms:title</plain-text-type>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_create', array(
            'plain_text_types' => array('dcterms:title'),
            'editor_base_path' => '/bundles/cmfcreate/vendor/ckeditor/',
            'fixed_toolbar'    => true,
            'stanbol_url'      => 'http://dev.iks-project.eu:8081',
        ));

``plain_text_types``
....................

A list of RDFa field types that will be edited with a plain text editor without
any formatting options. All other fields are edited with the WYSIWYG options
activated.

``editor_base_path``
....................

If you use a non-standard setup, you can adjust the editor base path
configuration. This is only relevant for CKEditor.

``fixed_toolbar``
.................

Fix the editor toolbar on top of the page. Currently only supported by the
hallo.js editor.

``stanbol_url``
...............

Apache stanbol can be used for semantic enhancement of content. This feature
can optionally be used with the hallo.js editor.
