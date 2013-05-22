Using a Custom Document Class Mapper with PHPCR-ODM
===================================================

The default document class mapper of PHPCR-ODM uses the attribute ``phpcr:class``
to store and retrieve the document class of a node. When accessing an existing
PHPCR repository, you might need different logic to decide on the class.

You can extend the ``DocumentClassMapper`` or implement ``DocumentClassMapperInterface``
from scratch. The important methods are getClassName that needs to find the class name
and writeMetadata that needs to make sure the class of a newly stored document can be
determined when loading it again.

Then you can overwrite the ``doctrine.odm_configuration`` service to call
``setDocumentClassMapper`` on it. An example from the `symfony cmf sandbox`_
(``magnolia_integration`` branch):

.. configuration-block::

    .. code-block:: yaml

        # if you want to overwrite default configuration, otherwise use a
        # custom name and specify in odm configuration block

        doctrine.odm_configuration:
            class: %doctrine_phpcr.odm.configuration.class%
            calls:
                - [ setDocumentClassMapper, [@sandbox_magnolia.odm_mapper] ]

        sandbox_magnolia.odm_mapper:
            class: "Sandbox\MagnoliaBundle\Document\MagnoliaDocumentClassMapper"
            arguments:
                - 'standard-templating-kit:pages/stkSection': 'Sandbox\MagnoliaBundle\Document\Section'

    .. code-block:: xml

        <service id="doctrine.odm_configuration" 
            class="%doctrine_phpcr.odm.configuration.class%">
            <call method="setDocumentClassMapper">
                <argument type="service" id="sandbox_magnolia.odm_mapper" />
            </call>
        </service>

        <service id="sandbox_magnolia.odm_mapper"
            class="Sandbox\MagnoliaBundle\Document\MagnoliaDocumentClassMapper">
            <argument type="collection">
                <argument type="standard-templating-kit:pages/stkSection">Sandbox\MagnoliaBundle\Document\Section</argument>
            </argument>
        </service>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;
        
        $container
            ->register('doctrine.odm_configuration', '%doctrine_phpcr.odm.configuration.class%')
            ->addMethodCall('setDocumentClassMapper', array(
                new Reference('sandbox_magnolia.odm_mapper'),
            ))
        ;

        $container ->setDefinition('sandbox_amgnolia.odm_mapper', new Definition(
            'Sandbox\MagnoliaBundle\Document\MagnoliaDocumentClassMapper',
            array(
                array(
                    'standard-templating-kit:pages/stkSection' => 'Sandbox\MagnoliaBundle\Document\Section',
                ),
            ),
        ));

Here you create a mapper that uses a configuration to read node information
and map that onto a document class.

If you have several repositories, you can use one configuration per
repository. See :ref:`bundle-phpcr-odm-multiple-phpcr-sessions`.

.. _`symfony cmf sandbox`: https://github.com/symfony-cmf/cmf-sandbox/tree/magnolia_integration
