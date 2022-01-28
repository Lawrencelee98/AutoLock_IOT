<template>
	<div class="attendance">
		<!-- 面包屑导航 -->
		<el-breadcrumb separator-class="el-icon-arrow-right">
			<el-breadcrumb-item :to="{ path: '/home' }">首页</el-breadcrumb-item>
			<el-breadcrumb-item>用户管理</el-breadcrumb-item>
			<el-breadcrumb-item>用户列表</el-breadcrumb-item>
		</el-breadcrumb>
		
		<!-- 卡片视图 -->
		<el-card class="box-card">
<!-- 			<el-row :gutter="5">
				<el-col :span="7">
					<el-input placeholder="请输入内容" v-model='queryInfo.query' clearable @clear=''>
						<el-button slot="append" icon="el-icon-search" @click=''></el-button>
					</el-input>
				</el-col>
				<el-col :span="4">
					<el-button type='primary' @click="">添加用户</el-button>
				</el-col>
			</el-row> -->
			<el-row :gutter="5">
				<el-col :span="4" :offset="21">
					<el-button type='primary' @click="getData()">Refresh</el-button>
				</el-col>
			</el-row>
			<!-- 用户列表区域 -->
			<el-table :data='timeList' border stripe>
				<el-table-column type='index' label='#'></el-table-column>
				<el-table-column label="Id" prop='id'></el-table-column>
				<el-table-column label="Name" prop='name'></el-table-column>
				<el-table-column label="Time" prop="date"></el-table-column>
			</el-table>
		</el-card>
	</div>
</template>

<script>
export default{
	data(){
		return {
			timeList:[
				{
					name:'Leee',
					id:'111',
					time:'2022-1-25 17:28'
				}
			]
		}
	},
	create(){
		getData();
	},
	methods:{
		async getData(){
			console.log("Get Data")
			const {data:res} = await this.$http.get('timelist')
			this.timeList = res
			
		}
		
	}
	
}
</script>

<style>
</style>
