<h1>Network Inventory System</h1>
The plugin provides inventory system for multiplayer sessions.
<h2>Prerequisites</h2>
Before using, you should be sure you're using unreal engine 5 (I mean strictly the fifth version).
<h2>Inventory Settings</h2>
There'ra special "Inventory Settings" in project settings (it is kind of developer settings) and works like a singletone.
<ul>
    <li>To use categorized inventory turn bIsCategorized on</li>
    <li>If you're using categories, you can add some category data assets in "Categories" array</li>
    <li>To use special log turn bUseInternalLogs on</li>
    <li>There's only one console command "InventorySystem.Debug.Show" that shows all inventory items</li>
</ul>
<h2>Use Steps</h2>
<ol>
    <li>Add inventory component to your Actor</li>
    <li>Create needed inventory item fragments</li>
    <li>Register your fragments in "Asset Manager" settings, these ones have asset type 'Item'</li>
</ol>
