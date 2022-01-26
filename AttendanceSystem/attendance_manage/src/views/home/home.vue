<template>
	<el-container class='home-container'>
		<el-header>
			<div>
				<img src="../../assets/logo.png" style="height: 100%;">
				<span>My Dashboard</span>
			</div>
			<el-button type='info'>
				退出
			</el-button>
		</el-header>
		<el-container>
			<el-aside width="200px">
				<el-menu router id='id-menu' background-color='#333744' text-color='#fff' active-text-color='#409EFF' :unique-opened='true' :default-active='activeIndex'>
<!-- 					<el-submenu index="0" style='padding-left: 0;'>
						<template slot='title'>
							<i class='el-icon-s-claim'></i>
							<span style="font-size: larger;">出勤管理</span>
						</template>
					</el-submenu> -->
					<el-menu-item index='/attendance'>
						<i class='el-icon-s-claim'></i>
						<span style="font-size: larger;" slot='title'>出勤管理</span>
					</el-menu-item>
				</el-menu>
				
			</el-aside>
			<el-main>
				<router-view></router-view>
			</el-main>
		</el-container>
	</el-container>
</template>

<script>
export default{
	data(){
		return {
			isCollapse:false,
			activeIndex: "/attendance",
		}
	},
	methods:{
		saveNavState(){
			
		}
	}
}
</script>

<style>
	/* 注意!!! 这里用类选择器 */
	.el-header{
		background-color: #373d41;
		display: flex;
		justify-content: space-between;
		padding-left: 0;
		align-items: center;
		color: #fff;
		font-size: 20px;
	}
	
	.el-header>div{
		height: 100%;
		display: flex;
		align-items: center;
	}
	
	.el-header>div>span{
		margin-left: 20px;
	}
	
	.el-aside{
		background-color: #333744;
	}
	
	.el-main{
		background-color: #EAEDF1;
	}
		
	.home-container{
		height: 100%;
	}
	
	.icon{
		padding-right: 30px;
	}
	
	#id-menu{
		border-right: none;
	}
	
	#id-submenu{
	}
	
	.toggle-button{
		background-color: #4A5064;
		font-size: 10px;
		line-height: 24px;
		color: #fff;
		text-align: center;
		letter-spacing: 0.2em;
		cursor: pointer;
	}
</style>

