CoreBundle
==========

The CoreBundle has no own documents that can be edited, but we provide admin
extensions under the core bundle namespace that are useful for all documents.

Admin extensions can be added to admin services through configuration in the
SonataAdminBundle. See the `Sonata Admin extension documentation`_ for more
information.

We provide the child extension to help with adding documents in overlay dialogs,
and extensions to edit publish workflow information.

Configuration
-------------

This section configures the admin extensions for publish workflow editing.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_sonata_phpcr_admin_integration:
            bundles:
                core:
                    extensions:
                        publishable:
                            form_group:           form.group_publish_workflow
                            form_tab:             form.tab_publish
                        publish_time:
                            form_group:           form.group_publish_workflow
                            form_tab:             form.tab_publish

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/sonata-phpcr-admin-integration">
                <bundles>
                    <extension>
                        <publishable
                            form-group="form.group_publish_workflow"
                            form-tab="form.tab_publish"
                        />
                        <publish-time
                            form-group="form.group_publish_workflow"
                            form-tab="form.tab_publish"
                        />
                    </extension>
                </bundles>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_sonata_phpcr_admin_integration', [
            'bundles' => [
                'core' => [
                    'extensions' => [
                        'publishable' => [
                            'form_group' => 'form.group_publish_workflow',
                            'form_tab' => 'form.tab_publish',
                        ],
                        'publish_time' => [
                            'form_group' => 'form.group_publish_workflow',
                            'form_tab' => 'form.tab_publish',
                        ],
                    ],
                ],
            ],
        ]);

Note that the child extension has no configuration and just can be attached to
admins as needed - see below for details.

``form_group``
~~~~~~~~~~~~~~

**type**: ``string`` **default**: as in above example.

Defines which form group the fields from this extension will appear in within
the Sonata Admin edit interface.

The caption for the group is determined by translating the group name. If you
change the group, be sure to also define a translation for it.

``form_tab``
~~~~~~~~~~~~

**type**: ``string`` **default**: as in above example.

Defines which form tab the fields from this extension will appear in within
the Sonata Admin edit interface.

The caption for the tab is determined by translating the tab name. If you
change the tab name, be sure to also define a translation for it.

Using Child Models: The Child Sonata Admin Extension
----------------------------------------------------

This extension sets a default parent to every new object instance if a
``parent`` parameter is present in the URL. The parent parameter is present
for example when adding documents in an overlay with the
``doctrine_phpcr_odm_tree_manager`` or when adding a document in the tree of
the dashboard.

To enable the extension in your admin classes, simply define the extension
configuration in the ``sonata_admin`` section of your project configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            # ...
            extensions:
                cmf_sonata_phpcr_admin_integration.core.extension.child:
                    implements:
                        - Symfony\Cmf\Bundle\CoreBundle\Model\ChildInterface
                        - Doctrine\ODM\PHPCR\HierarchyInterface

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://sonata-project.org/schema/dic/admin">
                <!-- ... -->
                <extension id="cmf_sonata_phpcr_admin_integration.core.extension.child">
                    <implement>Symfony\Cmf\Bundle\CoreBundle\Model\ChildInterface</implement>
                    <implement>Doctrine\ODM\PHPCR\HierarchyInterface</implement>
                </extension>
            </config>

        </container>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Cmf\Bundle\CoreBundle\Model\ChildInterface;
        use Doctrine\ODM\PHPCR\HierarchyInterface;

        $container->loadFromExtension('sonata_admin', [
            // ...
            'extensions' => [
                'cmf_sonata_phpcr_admin_integration.core.extension.child' => [
                    'implements' => [
                        ChildInterface::class,
                        HierarchyInterface::class,
                    ],
                ],
            ],
        ]);

Editing publication information: Publish Workflow Sonata Admin Extension
------------------------------------------------------------------------

When using the :doc:`write interface of the publish workflow <../core/publish_workflow>`,
this admin extension can be used to edit publication information.

To enable the extensions in your admin classes, define the extension
configuration in the ``sonata_admin`` section of your project configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            # ...
            extensions:
                cmf_sonata_phpcr_admin_integration.core.extension.publish_workflow.publishable:
                    implements:
                        - Symfony\Cmf\Bundle\CoreBundle\PublishWorkflow\PublishableInterface
                cmf_sonata_phpcr_admin_integration.core.extension.publish_workflow.time_period:
                    implements:
                        - Symfony\Cmf\Bundle\CoreBundle\PublishWorkflow\PublishTimePeriodInterface

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://sonata-project.org/schema/dic/admin">
                <!-- ... -->
                <extension id="cmf_sonata_phpcr_admin_integration.core.extension.publish_workflow.publishable">
                    <implement>
                        Symfony\Cmf\Bundle\CoreBundle\PublishWorkflow\PublishableInterface
                    </implement>
                </extension>

                <extension id="cmf_sonata_phpcr_admin_integration.core.extension.publish_workflow.time_period">
                    <implement>
                        Symfony\Cmf\Bundle\CoreBundle\PublishWorkflow\PublishTimePeriodInterface
                    </implement>
                </extension>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Cmf\Bundle\CoreBundle\PublishWorkflow\PublishableInterface;
        use Symfony\Cmf\Bundle\CoreBundle\PublishWorkflow\PublishTimePeriodInterface;

        $container->loadFromExtension('sonata_admin', [
            // ...
            'extensions' => [
                'cmf_sonata_phpcr_admin_integration.core.extension.publish_workflow.publishable' => [
                    'implements' => [
                        PublishableInterface::class,
                    ],
                ],
                'cmf_sonata_phpcr_admin_integration.core.extension.publish_workflow.time_period' => [
                    'implements' => [
                        PublishTimePeriodInterface::class,
                    ],
                ],
            ],
        ]);

.. _`Sonata Admin extension documentation`: https://sonata-project.org/bundles/admin/master/doc/reference/extensions.html
